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
        int rv = 0;
        if (jsonValue.isNull())
        {
            // TODO: Error Handling
        }
        try
        {
            rv = jsonValue.asInt();
        }
        catch (...)
        {
            // TODO: Error Handling
        }
        return rv;
    }
        
    std::string GatewayResponse::getResponseBody() const
    {
        auto jsonValue = subresponse["body"];
        std::string rv = std::string();
        if (jsonValue.isNull())
        {
            // TODO: Error Handling
        }
        try
        {
            rv = jsonValue.asString();
        }
        catch (...)
        {
            // TODO: Error Handling
        }
        return rv;
    }

    bool GatewayResponse::isResponseBodySet() const
    {
        auto jsonValue = subresponse["body"];
        return !jsonValue.isNull();
    }

    std::map<std::string, std::string> GatewayResponse::getJsonResponseHeader() const
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

    std::map<std::string, std::string> GatewayResponse::getRequestHeader() const
    {
        std::map<std::string, std::string> requestHeader;
        try {
            auto jsonHeaders = subrequest["headers"];
            if (!jsonHeaders.isNull())
            {
                for (auto i = jsonHeaders.begin(); i != jsonHeaders.end(); ++i)
                {
                    auto name = i.key().asString();
                    auto value = (*i)[static_cast<Json::Value::ArrayIndex>(0)].asString();
                    requestHeader[name] = value;
                }
            }
        }
        catch (...)
        {
            // TODO: Error Handling
        }
        return requestHeader;
    }
    
    std::map<std::string, std::string> GatewayResponse::getJsonResponseCookies() const
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