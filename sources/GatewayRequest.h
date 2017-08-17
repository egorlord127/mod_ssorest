#pragma once

#include "json/json.h"
#include <httpd.h>
#include <string>
namespace ssorest
{
    class GatewayRequest
    {
    private:
        server_rec* server;
        request_rec* request;
        std::string scheme;
        Json::Value jsonData;

    public:
        GatewayRequest(request_rec* sourceRequest);
        void buildJsonRequest();
        const Json::Value& getPayload() const;
        std::string sendTo(const std::string& gatewayUrl) const;
        void setAcoName(const std::string& value);
        void setPluginId(const std::string& value);
        void setRandomtext(const std::string& value);
        void setRandomtextSigned(const std::string& value);
        void setGatewayToken(const std::string& value);

    private:
        static std::string getScheme(const request_rec* request);
        static int getServerPort(const request_rec* request);
        void verifyThatSourceRequestIsDefined() const;
        bool isSecureProtocol() const;
    
    public:
        static const std::string acoName;
        static const std::string gatewayToken;
        static const std::string pluginId;
        static const std::string randomText;
        static const std::string randomTextSigned;
        static const std::string content;
        static const std::string contentTimestamp;
        static const std::vector<std::string> jsonAttributesKeys;

        void set(const std::string& key, const std::string& value);
        std::string get(const std::string& key) const;
        std::map<std::string, std::string> enumerateAttributes() const;
        static apr_status_t cleanupAttributes(void* attribute);
    };
}