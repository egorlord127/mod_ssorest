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
    };
}