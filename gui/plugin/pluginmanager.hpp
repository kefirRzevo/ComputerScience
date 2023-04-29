#pragma once


#include <vector>

#include "plugin.hpp"

using namespace plugin;


class Plugin final
{
    public:

        Plugin(const char* pluginPath, IAPI* iApi);

        ~Plugin();

        const std::string&
        GetName() const;

        IPlugin* GetIPlugin();

    private:

        void
        Initialize();

        CreateFunction  create;
        DestroyFunction destroy;

        std::string name;
        IPlugin* iPlugin;
        void* handle;
};

class PluginManager
{
    private:

        PluginManager();

    public:

        ~PluginManager();

        static PluginManager*
        Get();

        const std::vector<Plugin*>&
        GetPlugins() const;

    private:

        std::vector<Plugin*> loadedPlugins;
        IAPI* iApi;

        static std::unique_ptr<PluginManager> pluginManager;
};
