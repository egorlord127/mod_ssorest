#pragma once

#include "json/json.h"
#include <httpd.h>
#include <string>
namespace ssorest
{
    class GatewayRequest
    {
    private:
        const server_rec* server;
        std::string scheme;
        Json::Value jsonData;

    public:
        GatewayRequest(const request_rec* sourceRequest, const std::string& fqdn);
        const Json::Value& getPayload() const;
        std::string sendTo(const std::string& gatewayUrl) const;
    
    private:
        static std::string getScheme(const request_rec* request);
        static int getServerPort(const request_rec* request);
        void verifyThatSourceRequestIsDefined() const;
        bool isSecureProtocol() const;
        std::string getContextPath(const request_rec* request) const;
        // static std::vector<Cookie> extractCookiesFrom(CurlWrapper& curl);
    };
}