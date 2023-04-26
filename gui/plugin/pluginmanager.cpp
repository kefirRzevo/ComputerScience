#include <filesystem>
#include <vector>
#include <dlfcn.h>

#include "pluginmanager.hpp"
#include "api.hpp"


std::unique_ptr<PluginManager> PluginManager::pluginManager;

//----------------------------------------//

Plugin::Plugin(const char* pluginPath, IAPI* iApi):
create(nullptr), destroy(nullptr), iPlugin(nullptr), handle(nullptr)
{
    handle = dlopen(pluginPath, RTLD_NOW);
    if(!handle)
    {
        fprintf(stderr, "Error from file %s. %s.\n", pluginPath, dlerror());
        return;
    }

    Initialize();

    if(create)
        iPlugin = create(iApi);
}

void
Plugin::Initialize()
{
    create = (CreateFunction)dlsym(handle, "Create");
    if(!create)
    {
        fprintf(stderr, "Error happened with Create Function\n");
        return;
    }
    destroy = (DestroyFunction)dlsym(handle, "Destroy");
    if(!destroy)
    {
        fprintf(stderr, "Error happened with Destroy Function\n");
        return;
    }
}

Plugin::~Plugin()
{
    if(iPlugin && destroy)
        destroy(iPlugin);
    
    if(handle)
        dlclose(handle);
}

IPlugin*
Plugin::GetIPlugin()
{
    return iPlugin;
}

//----------------------------------------//

PluginManager::PluginManager():
iApi(new API{})
{
    std::filesystem::path pluginPath;
    std::string pluginPathString;
    std::string pluginDir = Config::defSoPluginDir;

    for(auto& plugin: std::filesystem::directory_iterator(pluginDir))
    {
        pluginPath       = plugin;
        pluginPathString = pluginPath.std::filesystem::path::generic_string();
        loadedPlugins.push_back(new Plugin{pluginPathString.c_str(), iApi});
    }
}

PluginManager::~PluginManager()
{
    if(iApi)
        delete iApi;

    for(auto plugin: loadedPlugins)
        delete plugin;
}

PluginManager*
PluginManager::Get()
{
    if(pluginManager.get() == nullptr)
        pluginManager.reset(new PluginManager{});

    return pluginManager.get();
}

const std::vector<Plugin*>&
PluginManager::GetPlugins() const
{
    return loadedPlugins;
}

//----------------------------------------//
