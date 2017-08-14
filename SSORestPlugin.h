#pragma once

#include <httpd.h>
#include <memory>
#include <string>

namespace ssorest
{
    class SSORestPlugin
    {
    private:
        bool isEnabled;
        std::string accountName;
        std::string gatewayUrl;
        std::string pluginIdKey;
        std::string secretKey;
               
    public:
        SSORestPlugin();

    public:
        void enable();
        void disable();
        void setAccountName(const std::string& value);
        void setGatewayUrl(const std::string& value);
        void setPluginId(const std::string& value);
        void setSecretKey(const std::string& value);
        const bool& getEnable();
    };
}