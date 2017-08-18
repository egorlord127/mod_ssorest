#include "GatewayResponse.h"
namespace ssorest
{
    GatewayResponse::GatewayResponse(const std::string& jsonResponse)
    {
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(jsonResponse, root))
        {
            // TODO: Handling Exception
        }
        response = root;
        subrequest = root["request"];
        subresponse = root["response"];
    }

    const Json::Value& GatewayResponse::getJsonResponse() const
    {
        return response;
    }

    const Json::Value& GatewayResponse::getJsonSubRequest() const
    {
        return subrequest;
    }
        
    const Json::Value& GatewayResponse::getJsonSubResponse() const
    {
        return subresponse;
    }
    
    int GatewayResponse::getJsonResponseStatus() const
    {
        auto jsonValue = subresponse["status"];
        if (jsonValue.isNull())
        {
            // TODO: Error Handling
        }
        try
        {
            return jsonValue.asInt();
        }
        catch (...)
        {
            // TODO: Error Handling
        }
    }
        
    std::string GatewayResponse::getResponseBody() const
    {
        auto jsonValue = subresponse["body"];
        if (jsonValue.isNull())
        {
            // TODO: Error Handling
        }
        try
        {
            return jsonValue.asString();
        }
        catch (...)
        {
            // TODO: Error Handling
        }
    }

    std::map<std::string, std::string> GatewayResponse::getResponseHeader() const
    {
        std::map<std::string, std::string> responseHeader;
        try {
            auto jsonHeaders = subresponse["headers"];
            if (!jsonHeaders.isNull())
            {
                for (auto i = jsonHeaders.begin(); i != jsonHeaders.end(); ++i)
                {
                    auto name = i.key().asString();
                    auto value = (*i)[static_cast<Json::Value::ArrayIndex>(0)].asString();
                    responseHeader[name] = value;
                }
            }
        }
        catch (...)
        {
            // TODO: Error Handling
        }
        return responseHeader;
    }
        
    std::map<std::string, std::string> GatewayResponse::getResponseCookies() const
    {
        std::map<std::string, std::string> cookies;
        try {
            auto jsonCookies = subresponse["cookies"];
            if (!jsonCookies.isNull())
            {
                for (auto i = jsonCookies.begin(); i != jsonCookies.end(); ++i)   
                {
                    auto name = i.key().asString();
                    auto value = (*i)[static_cast<Json::Value::ArrayIndex>(0)].asString();
                    cookies[name] = value;
                }
            }
        }
        catch (...)
        {
            // TODO: Error Handling
        }

        return cookies;
    }
}