#ifndef GARBAGE_PLUGIN_FRAMEWORK_PLUGINS_MANAGER_H
#define GARBAGE_PLUGIN_FRAMEWORK_PLUGINS_MANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "Plugin.h"

namespace GPF
{

    class PluginsManager final
    {
    public:

        using PluginsMap = std::unordered_map<std::string, std::unique_ptr<Plugin, std::function<void(Plugin*)>>>;

        Plugin* Load(const std::string& name);
        Plugin* Find(const std::string& name);

        void Unload(const std::string& name);

        PluginsMap::const_iterator begin() { return m_loadedPlugins.cbegin(); }
        PluginsMap::const_iterator end() { return m_loadedPlugins.cend(); }

    private:

        std::unordered_map<std::string, std::unique_ptr<Plugin, std::function<void(Plugin*)>>> m_loadedPlugins;

    };

}

#endif