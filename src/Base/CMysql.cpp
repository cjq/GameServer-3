#include "CMysql.h"
#include <stdlib.h>
#include "PIGlobalStruct.h"

extern "C" {
#include "sh.h"
}

CMysql::CMysql()
{
    m_strHost = "localhost";
    m_strUser = "root";
    m_strPassword = "";
    m_dwPort = 3306;
    m_strName = "xproject";
    memset(m_strQuery, 0, MAX_QUERY_SIZE);
}

CMysql::CMysql(const char *host, const char *user, const char *password, const char *name, int port)
{
    m_strHost = host;
    m_strUser = user;
    m_strPassword = password;
    m_dwPort = port;
    m_strName = name;
    memset(m_strQuery, 0, MAX_QUERY_SIZE);
}

CMysql::~CMysql()
{

}
/************************************************************************/
/* 连接数据库                                                                     */
/************************************************************************/
int CMysql::DbConnect()
{
    m_pMysql.disable_exceptions();
    m_pMysql.set_option(new SetCharsetNameOption("utf8"));
    if(m_pMysql.connect(m_strName.c_str(), m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_dwPort))
	{
        DbQuery("SET NAMES UTF8");
		sh_info("CMysql::CMysql() Connect to host %s with user name %s and password %s dbname %s success!", m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_strName.c_str());
        return 0;
    }
	sh_warning("CMysql::CMysql() Connect to host %s with user name %s and password %s dbname %s fail!", m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), m_strName.c_str());
    sh_error("DB connection failed:%s\n", getDbLastError());
    return -1;
}

INT32	CMysql::ExecQuery(const char *cpsSQL, StoreQueryResult &rsResult)
{
	Query query = m_pMysql.query(cpsSQL);
	rsResult = query.store();

	return eNormal;
}

/************************************************************************/
/* 执行数据库语句（查询）                                               */
/************************************************************************/
StoreQueryResult CMysql::DbQuery(const char *sql)
{
    StoreQueryResult res;
    if (strlen(sql) > 0)
    {
        Query query = m_pMysql.query(sql);
        if(!(res = query.store()))
        {            
            sh_error("Mysql query error: %s", m_pMysql.error());
        }
    }
    return res;
}
/************************************************************************/
/* 执行写入操作(update、insert、delete)                                                                     */
/************************************************************************/
INT64 CMysql::DbExecute(const char *sql, eMysqlQueryType queryType)
{
    sh_trace("mysql exec sql %s", sql);
    SimpleResult res;
    INT64 affectedNumRows = 0;
    Query query = m_pMysql.query();
    query.disable_exceptions();
    query << sql;
    res = query.execute();

    switch(queryType){

    case eMysqlProcedure:           //存储过程
    case eMysqlDelete:                  //删除
    case eMysqlUpdate:      //添加和删除返回响应记录数
       if(m_pMysql.errnum())
	   {
           affectedNumRows = -1;
       }
        break;
    case eMysqlInsert:          //新增记录返回自增id
        affectedNumRows = res.insert_id();
        if (m_pMysql.errnum())
        {
            affectedNumRows = -1;
        }
        break;
    default:
        break;
    }
	
    if (affectedNumRows == -1)
    {
        sh_error("Mysql exec error: %s", getDbLastError());
        sh_error("Mysql last query: %s", GetLastQuerySql().c_str());
    }
    return affectedNumRows;
}

/************************************************************************/
/* 获取最后一次执行错误信息                                             */
/************************************************************************/
const char *CMysql::getDbLastError()
{
    return m_pMysql.error();
}

/************************************************************************/
/* 查询数据                                                                     */
/************************************************************************/
StoreQueryResult CMysql::DbSelect(const char *tableName, map<string, string> &fields, map<string, string> &condition,unsigned long offSet, unsigned long limit, string &orderBy, string &groupBy)
{
    memset(m_strQuery, 0, MAX_QUERY_SIZE);
    string orderString("");
    string groupString("");
    char limitChr[SMALL_MSG_SIZE];
    memset(limitChr, 0, SMALL_MSG_SIZE);
    if (!orderBy.empty())
    {
        orderString = "ORDER BY  "+ orderBy;
    }
    if(!groupBy.empty())
    {
        groupString = "GROUP BY " + groupBy;
    }

    if ((limit != 0) && (offSet >= 0))
    {
        snprintf(limitChr, sizeof(limitChr), "LIMIT %d, %d", (int)offSet, (int)limit);
    }

    string fieldString("");
    fieldString = MakeSelect(fields);
    string conditionString = MakeCondition(condition);
    snprintf(m_strQuery, sizeof(limitChr), "SELECT %s FROM %s WHERE %s %s %s %s",fieldString.c_str() , tableName, conditionString.c_str(), groupString.c_str(), orderString.c_str(), limitChr);
    return DbQuery(m_strQuery);
}
/************************************************************************/
/* 更新数据                                                             */
/************************************************************************/
INT64 CMysql::DbUpdate(const char *tableName, map<string, string> &fields, map<string, string> &condition)
{
    memset(m_strQuery, 0, MAX_QUERY_SIZE);

    string fieldString("");
    fieldString = MakeUpdate(fields);
    string conditionString = MakeCondition(condition);
    snprintf(m_strQuery, sizeof(m_strQuery), "UPDATE %s SET %s WHERE %s", tableName, fieldString.c_str(), conditionString.c_str());
    return DbExecute(m_strQuery, eMysqlUpdate);
}
/************************************************************************/
/* 添加纪录                                                                     */
/************************************************************************/
INT64 CMysql::DbInsert(const char *tableName, map<string, string> &fields)
{
    memset(m_strQuery, 0, MAX_QUERY_SIZE);

    string fieldString("");
    fieldString = MakeInsert(fields);
    snprintf(m_strQuery, sizeof(m_strQuery), "INSERT INTO %s %s", tableName, fieldString.c_str());
   return DbExecute(m_strQuery, eMysqlInsert);   
}
/************************************************************************/
/* 删除记录                                                                     */
/************************************************************************/
INT64 CMysql::DbDelete(const char *tableName, map<string, string> &condition)
{
    memset(m_strQuery, 0, MAX_QUERY_SIZE);
    string conditionString = MakeCondition(condition);
    snprintf(m_strQuery, sizeof(m_strQuery), "DELETE FROM %s WHERE %s",tableName, conditionString.c_str());
    return DbExecute(m_strQuery, eMysqlDelete);
}
/************************************************************************/
/* 插入记录,如果存在就更新                                              */
/************************************************************************/
INT64 CMysql::DbInsertUpdate(const char *tableName, map<string, string> &fields)
{
	//memset(m_strQuery, 0, MAX_QUERY_SIZE);
    CHAR aszSQLBuff[1024 * 1024];
	aszSQLBuff[0] = '\0';
	string fieldString1("");
	string filedString2("");
	fieldString1 = MakeInsert(fields);
	filedString2 = MakeInsertUpdate(fields);
	//UINT32 un32Str1Len = fieldString1.length();
	//UINT32 un32Str2Len = filedString2.length();
	snprintf(aszSQLBuff, sizeof(aszSQLBuff), "INSERT INTO %s %s on duplicate key update %s", tableName, fieldString1.c_str(), filedString2.c_str());
	//UINT32 un32SQLLen = strlen(aszSQLBuff);
	return DbExecute(aszSQLBuff, eMysqlInsert);
}
/************************************************************************/
/* 构造查询字段数据                                                     */
/************************************************************************/
string CMysql::MakeSelect(map<string, string> &fields)
{
    string strField("");
    bool isFirst = true;
    for (map<string, string>::iterator iter = fields.begin(); iter != fields.end(); ++ iter)
    {
        if (!isFirst)
        {
            strField += ",";
        }
        isFirst = false;
        strField += iter->first;
    }
    return strField;
}
/************************************************************************/
/* 构造插入语句                                                                     */
/************************************************************************/
string CMysql::MakeInsert(map<string, string> &fields)
{
    string strFieldName("");
    string strFieldValue("");
    string strField("");
    bool isFirst = true;
    for (map<string, string>::iterator iter = fields.begin(); iter != fields.end(); ++ iter)
    {
        if (!isFirst)
        {
            strFieldName += ",";
            strFieldValue += ",";
        }
        isFirst = false;
        strFieldName += iter->first;
       static char value[LARGE_MSG_SIZE];
        memset(value, 0, LARGE_MSG_SIZE);
        mysql_escape_string(value, iter->second.c_str(), iter->second.length());
        strFieldValue += "'"+string(value)+"'";
    }
    strField = "(" + strFieldName + ")VALUES(" + strFieldValue + ")";
    return strField;
}
/************************************************************************/
/* 构造插入已存在记录时更新语句                                              */
/************************************************************************/
string CMysql::MakeInsertUpdate(map<string, string> &fields)
{
	string strField(""); 
	bool isFirst = true;
	for (map<string, string>::iterator iter = fields.begin(); iter != fields.end(); ++ iter)
	{
		if (!isFirst)
		{
			strField += ",";
		}
		isFirst = false;
		strField += iter->first;
		strField += '=';
		char value[LARGE_MSG_SIZE];
		memset(value, 0, LARGE_MSG_SIZE);
		mysql_escape_string(value, iter->second.c_str(), iter->second.length());
		strField += "'"+ string(value)+"'";
	}
	return strField;
}
/************************************************************************/
/* 构造更新语句                                                         */
/************************************************************************/
string CMysql::MakeUpdate(map<string, string> &fields)
{
    string strField("");
    string fieldValue;
	string fieldName;
    bool isFirst = true;
    for (map<string, string>::iterator iter = fields.begin(); iter != fields.end(); ++ iter)
    {
        if (!isFirst)
        {
            strField += ",";
        }
        isFirst = false;

        char value[LARGE_MSG_SIZE];
		char field[SMALL_MSG_SIZE];
		memset(field, 0, SMALL_MSG_SIZE);
        memset(value, 0, LARGE_MSG_SIZE);
        mysql_escape_string(value, iter->second.c_str(), iter->second.length());
		mysql_escape_string(field, iter->first.c_str(), iter->first.length());

        fieldValue = string(value);
		fieldName = string(field);

		if (fieldName.find("=") != string::npos)
		{
			strField += fieldName;
		}
        else
		{
			if (fieldValue.find(iter->first) != string::npos)
			{
				strField += iter->first + "=" + fieldValue;
			}		
			else
			{
				strField += iter->first + "='" + fieldValue + "'";
			}
		}
    }
    return strField;
}
/************************************************************************/
/* 构造条件语句                                                                     */
/************************************************************************/
string CMysql::MakeCondition(map<string, string> &conditions)
{
    string strCondition("");
    bool isFirst = true;
    for (map<string, string>::iterator iter = conditions.begin(); iter != conditions.end(); ++ iter)
    {
        if (!isFirst)
        {
            strCondition += " AND ";
        }
        if ((iter->second.find("<") != string::npos) ||
            (iter->second.find("<=") != string::npos) || 
            (iter->second.find(">") != string::npos) || 
            (iter->second.find(">=") != string::npos) || 
            (iter->second.find("=") != string::npos) ||
            (iter->second.find("LIKE") != string::npos) ||
            (iter->second.find("IN") != string::npos) ||
            (iter->second.find("OR") != string::npos) ||
            (iter->second.find("AND") != string::npos)||
            (iter->second.length() == 0))
        {
            strCondition += "(" + iter->first + iter->second + ")";
        }
        else
        {
            strCondition += "(" + iter->first +" = '" + iter->second + "')";
        }
        isFirst = false;
    }

    if (strCondition.length() == 0)
    {
        strCondition = "1";
    }
    return strCondition;
}
string CMysql::GetLastQuerySql()
{
    return string(m_strQuery);
}

void CMysql::DbPing()
{
    m_pMysql.ping();
}
