#ifndef IMODULE_H
#define IMODULE_H

class IPluginManager;

class IModule
{
public:
    IModule()
    {
	   bCanReload = true;
    }

    virtual ~IModule() {}

    virtual bool Init() {return true;}

    virtual bool AfterInit() {return true;}

    virtual bool BeforeShut() {return true;}

    virtual bool Shut() {return true;}

    virtual bool Execute() {return true;}

    virtual void OnReload(const char* strModuleName, IModule* pModule)
    {
    	BeforeShut();
    	Shut();

    	Init();
    	AfterInit();
    }

    virtual IPluginManager* GetPluginManager() const
    {
	   return pPluginManager;
    }

    std::string strName;
    bool bCanReload;

protected:
    IPluginManager* pPluginManager;
};



#endif
