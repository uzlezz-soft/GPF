#include "GPF/PluginsManager.h"

#ifdef GARBAGE_PLATFORM_WINDOWS
#include <windows.h>
#elif defined(GARBAGE_PLATFORM_LINUX)
#include <dlfcn.h>
#endif

static void* LoadFunction(const std::string& name, void* handle) noexcept
{
#ifdef GARBAGE_PLATFORM_WINDOWS
    return GetProcAddress(reinterpret_cast<HMODULE>(handle), name.c_str());
#elif defined(GARBAGE_PLATFORM_LINUX)
    return dlsym(handle, name.c_str());
#else
    return nullptr;
#endif
}

namespace GPF
{

    Plugin* PluginsManager::Load(const std::string& name)
    {
        #ifdef GARBAGE_PLATFORM_WINDOWS
        void* handle = reinterpret_cast<void*>(LoadLibraryA(name.c_str()));
        #elif GARBAGE_PLATFORM_LINUX
        void* handle = dlopen(name.c_str(), RTLD_LAZY);
        #else
        return nullptr;
        #endif

        if (!handle) return nullptr;

        auto createInstanceFunction = reinterpret_cast<GPF::Plugin*(*)()>(LoadFunction("__GPF_CreatePluginInstance", handle));
        if (!createInstanceFunction) return nullptr;

        Plugin* instance = createInstanceFunction();
        if (!instance) return nullptr;

        instance->m_handle = handle;

        m_loadedPlugins.insert(std::make_pair(name, std::unique_ptr<Plugin, std::function<void(Plugin*)>>(instance, 
            [](Plugin* plugin) 
            { 
                plugin->Unload(); 

                auto handle = plugin->GetInternalHandle();

                auto deleteInstanceFunction = reinterpret_cast<void(*)(GPF::Plugin*)>(LoadFunction("__GPF_DeletePluginInstance", plugin->GetInternalHandle()));
                if (deleteInstanceFunction) deleteInstanceFunction(plugin);

                #ifdef GARBAGE_PLATFORM_WINDOWS
                FreeLibrary(reinterpret_cast<HMODULE>(handle));
                #elif defined(GARBAGE_PLATFORM_LINUX)
                dlclose(handle);
                #endif
            })));

        instance->Load();
        return instance;
    }

    Plugin* PluginsManager::Find(const std::string& name)
    {
        auto& pair = m_loadedPlugins.find(name);
        if (pair != m_loadedPlugins.end()) return pair->second.get();

        for (auto& pluginPair : m_loadedPlugins)
        {
            auto plugin = pluginPair.second.get();

            if (plugin->GetName() == name) return plugin;
        }

        return nullptr;
    }

    void PluginsManager::Unload(const std::string& name)
    {
        auto& pair = m_loadedPlugins.find(name);
        if (pair != m_loadedPlugins.end())
        {
            m_loadedPlugins.erase(pair);
        }
    }

}