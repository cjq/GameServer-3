#ifndef _CMysql_
#define _CMysql_

#include "PIGlobalType.h"
#include "PIGlobalDef.h"
#include <stdio.h>
#include <iostream>
#include <mysql.h>
#include <mysql++.h>

using namespace std;
using namespace mysqlpp;


#define MAX_QUERY_SIZE 8192

class CMysql
{
//����
public:
    DB_Vector rows;

//����
public:
    CMysql();
    CMysql(const char *host, const char *user, const char *password, const char *name, int port);
    ~CMysql();
    //static CMysql *getDbInstance();
    //�������ݿ�
    int DbConnect();
    //�Ͽ����ݿ�����
    void DbDisConnect() { m_pMysql.disconnect(); }
    //ִ��sql
	INT32	ExecQuery(const char *cpsSQL, StoreQueryResult &rsResult);
    StoreQueryResult DbQuery(const char *sql);
    INT64 DbExecute(const char *sql, eMysqlQueryType queryType);
    //��ѯ��¼
    StoreQueryResult DbSelect(const char *tableName, map<string, string> &fields, map<string, string> &condition, unsigned long offSet, unsigned long limit, string &orderBy,  string &groupBy);
    //��������
    INT64 DbInsert(const char *tableName, map<string, string> &fields);
    //ɾ����¼
    INT64 DbDelete(const char *tableName,map<string, string> &condition);
    //���¼�¼
    INT64 DbUpdate(const char *tableName, map<string, string> &fields, map<string, string> &condition);
	//�����¼,������ھ͸���
	INT64 DbInsertUpdate(const char *tableName, map<string, string> &fields);
    //��ȡ���һ�γ�����Ϣ
    const char *getDbLastError();
    //��ȡ���һ�β�������ID
    UINT64 getDbLastInsertId();
    //��ȡ���һ��ִ�����
    string GetLastQuerySql();
    //ping
    void DbPing();

    string getHostName() { return m_strHost; };
    string getUser() { return m_strUser; };
    string getPassword() { return m_strPassword; };
    string getName() { return m_strName; };
    DWORD getPort() { return m_dwPort; };

	Connection		GetConnection(){return m_pMysql;}
//���Զ���
private:
    hash_map<string, string> m_row;
    //��¼��
    vector<hash_map<string, string> > m_rows;
    //���ݿ����Ӿ��
    Connection m_pMysql;
    //static CMysql *instance;
    //ִ��sql���
    char m_strQuery[MAX_QUERY_SIZE];
    //CGameConfig m_GameConfig;
    //���ݿ������
    string m_strHost;
    //�û���
    string m_strUser;
    //����
    string m_strPassword;
    //���ݿ���
    string m_strName;
    //�˿�
    DWORD m_dwPort;
//����
private:
    string MakeFields(map<string, string> &fields, eMysqlQueryType eType);
    string MakeSelect(map<string, string> &fields);
    string MakeInsert(map<string, string> &fields);
    string MakeUpdate(map<string, string> &fields);

private:
    string MakeCondition(map<string, string> &condition);
private:
    string MakeInsertUpdate(map<string, string> &fields);
    //void Init();
};

#endif
