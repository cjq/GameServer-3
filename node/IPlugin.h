#ifndef IPLUGIN_H
#define IPLUGIN_H

class IPlugin
{
public:
    virtual const int GetPluginVersion() = 0;

    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;

    virtual void AfterInit()
    {

    }

    virtual bool Execute()
    {
	
    }


};


class NFIPluginManager;

class NFIPlugin : public NFILogicModule,
    public NFMap<std::string, NFILogicModule>
{

public:

    virtual const int GetPluginVersion() = 0;

    virtual const std::string GetPluginName() = 0;

    virtual void Install() = 0;

    virtual bool Init()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->Init();
            if (!bRet)
            {
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool AfterInit()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            bool bRet = pModule->AfterInit();
            if (!bRet)
            {
                assert(0);
            }

            pModule = Next();
        }
        return true;
    }

    virtual bool CheckConfig()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->CheckConfig();

            pModule = Next();
        }

        return true;
    }

    virtual bool Execute()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->Execute();

            pModule = Next();
        }

        return true;
    }

    virtual bool BeforeShut()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->BeforeShut();

            pModule = Next();
        }
        return true;
    }

    virtual bool Shut()
    {
        NFILogicModule* pModule = First();
        while (pModule)
        {
            pModule->Shut();

            pModule = Next();
        }

        return true;
    }

    virtual void Uninstall() = 0;

protected:
    NFIPluginManager* pPluginManager;
};



#endif
