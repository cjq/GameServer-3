
#ifndef IPLUGIN_MANAGER_H
#define IPLUGIN_MANAGER_H

#include <map>
#include <list>
#include <string>
#include <stdio.h>
#include <iostream>
// using namespace std;



// #include "IModule.h"

class DynLib;
class IModule;
class IPlugin;

class PluginManager
{
public:
    PluginManager()
    {

    }
    


    bool Init();
    bool AfterInit();
    bool BeforeShut();
    bool Shut();
    bool Execute();

public:

    bool LoadPlugin();
    void RegisterPlugin(IPlugin* plugin);
    void UnRegisterPlugin(IPlugin* plugin);
    IPlugin* FindPlugin(const std::string& strPluginName);   

    void AddModule(const std::string& strModuleName, IModule* pModule);
    void RemoveModule(const std::string& strModuleName);
    IModule* FindModule(const std::string& strModuleName);

protected:
    bool LoadPluginLibrary(const std::string& strPluginDLLName);
    bool UnLoadPluginLibrary(const std::string& strPluginDLLName);
public:
    

private:
    typedef std::map<std::string, bool> PluginNameMap;
    typedef std::map<std::string, DynLib*> PluginLibMap;
    typedef std::map<std::string, IPlugin*> PluginInstaceMap;
    typedef std::map<std::string, IModule*> ModuleInstanceMap;

    typedef void(* DLL_START_PLUGIN_FUNC)(PluginManager* pm);
    typedef void(* DLL_STOP_PLUGIN_FUNC)(PluginManager* pm);

    PluginNameMap m_PluginNameMap;
    PluginLibMap m_PluginLibMap;
    PluginInstaceMap m_PluginInstanceMap;
    ModuleInstanceMap m_ModuleInstanceMap;
};

#endif
