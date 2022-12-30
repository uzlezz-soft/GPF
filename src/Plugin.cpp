#include "GPF/Plugin.h"

namespace GPF
{

    void Plugin::Init(const std::string& name)
    {
        if (m_initialized) return;

        m_name = name;
        m_initialized = true;
    }

}