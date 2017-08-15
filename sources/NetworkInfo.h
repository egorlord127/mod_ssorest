#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "PlatformInfo.h"

namespace ssorest
{
    class NetworkInfo
    {
    private:
#ifdef SSORestPlugin_LINUX
        std::mutex networkApiMutex;
#endif

    public:
        std::string getFQDN();
    };
}
