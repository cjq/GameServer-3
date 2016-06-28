#ifndef DYN_LIB_H
#define DYN_LIB_H

#include <stdio.h>
#include <iostream>

#include DYNLIB_HANDLE void*
#define DYNLIB_LOAD(a) dlopen(a, RTLD_LAZY | RTLD_GLOBAL)
#define DYNLIB_GETSYM(a, b) dlsym(a, b)
#define DYNLIB_UNLOAD(a) dlclose(a)


class DynLib
{
public:
    DynLib(const std::string& strName):m_hInst(NULL)
    {
    	m_strName = strName;
    	m_strName.append(".so");
    	printf("LoadPlugin:%s\n", m_strName.c_str());
    }
    
    ~DynLib() 
    {
    
    }

    bool Load()
    {
    	std::string strLibPath = "./";
    	strLibPath += m_strName;
    	m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strLibPath.c_str());

    	return m_hInst != NULL;
    }

    bool UnLoad()
    {
    	DYNLIB_UNLOAD(m_hInst);
    	return true;
    }

    const std::string& GetName(void) const
    {
	return m_strName;
    }
    
    void* GetSymbol(const char* szProcName)
    {
	   return (DYNLIB_HANDLE)DYNLIB_GETSYM(m_hInst, szProcName);
    }


private:
    std::string m_strName;
    DYNLIB_HANDLE m_hInst;
};

#endif
