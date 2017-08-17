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
}