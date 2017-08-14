#pragma once

#include <httpd.h>
#include <memory>
#include <string>
#include <vector>

namespace ssorest
{
    class SSORestPlugin
    {
    private:
        bool   isEnabled;
        bool   traceEnabled;
        std::string acoName;
        std::string gatewayUrl;
        std::string gatewayToken;
        std::string pluginId;
        std::string secretKey;
        std::vector<std::string> ssoZone;
        std::vector<std::string> ignoreExt;
        std::vector<std::string> ignoreUrl;
               
    public:
        SSORestPlugin();

    public:

        bool   getEnable();
        bool   getTraceEnable();
        std::string getAccountName();
        std::string getGatewayUrl();
        std::string getGatewayToken();
        std::string getPluginId();
        std::string getSecretKey();
        std::vector<std::string> getSSOZone();
        std::vector<std::string> getIgnoreExt();
        std::vector<std::string> getIgnoreUrl();

        void setEnable(const bool& flag);
        void setTraceEnable(const bool& flag);
        void setAccountName(const std::string& value);
        void setGatewayUrl(const std::string& value);
        void setGatewayToken(const std::string& value);
        void setPluginId(const std::string& value);
        void setSecretKey(const std::string& value);
        void addSSOZone(const std::string& value);
        void addIgnoreExt(const std::string& value);
        void addIgnoreUrl(const std::string& value);

    };
}