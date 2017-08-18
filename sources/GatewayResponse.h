#pragma once

#include "json/json.h"
#include <map>
#include <string>

namespace ssorest
{
    class GatewayResponse
    {
    private:
        Json::Value response;
        Json::Value subrequest;
        Json::Value subresponse;
        
    public:
        GatewayResponse(const std::string& jsonResponse);

    public:
        const Json::Value& getJsonResponse() const;
        const Json::Value& getJsonSubRequest() const;
        const Json::Value& getJsonSubResponse() const;

        int getJsonResponseStatus() const;
        std::string getResponseBody() const;
        
        std::map<std::string, std::string> getResponseHeader() const;
        std::map<std::string, std::string> getResponseCookies() const;
    };
}
