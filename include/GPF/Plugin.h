#ifndef GARBAGE_PLUGIN_FRAMEWORK_PLUGIN_H
#define GARBAGE_PLUGIN_FRAMEWORK_PLUGIN_H

#include "PlatformDetection.h"
#include <string>

#define GPF_PLUGIN_BODY(PLUGIN_CLASS, NAME) \
PLUGIN_CLASS() { Init(#NAME); } \
~PLUGIN_CLASS() {}

#ifdef GARBAGE_PLATFORM_WINDOWS
#define GPF_PLUGIN_GLUE(PLUGIN_CLASS) \
extern "C" __declspec(dllexport) GPF::Plugin* __GPF_CreatePluginInstance() { return new PLUGIN_CLASS; } \
extern "C" __declspec(dllexport) void __GPF_DeletePluginInstance(GPF::Plugin* plugin) { delete plugin; }
#else
#define GPF_PLUGIN_GLUE(PLUGIN_CLASS) \
extern "C" GPF::Plugin* __GPF_CreatePluginInstance() { return new PLUGIN_CLASS; } \
extern "C" void __GPF_DeletePluginInstance(GPF::Plugin* plugin) { delete plugin; }
#endif

namespace GPF
{

    // Base class from which you should derive your plugin class
    class Plugin
    {
    public:

        virtual ~Plugin() = default;

        virtual void Load() {}
        virtual void Unload() {}

        const std::string& GetName() const { return m_name; }

        void* GetInternalHandle() const { return m_handle; }

    protected:

        // For internal use only
        void Init(const std::string& name);

    private:

        friend class PluginsManager;

        bool m_initialized = false;
        std::string m_name;

        void* m_handle{nullptr};

    };

}

#endif