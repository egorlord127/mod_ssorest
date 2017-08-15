#include "GatewayRequest.h"
#include "TypesConverter.h"
#include "StringBuilder.h"

#include <http_request.h>
#include <http_core.h>
namespace ssorest
{
    GatewayRequest::GatewayRequest(const request_rec* sourceRequest, const std::string& fqdn)
    : server(sourceRequest->server)
    , scheme(getScheme(sourceRequest))
    {
        // TODO: 
        // method
        auto requestMethod = TypesConverter::toStringSafety(sourceRequest->method);
        jsonData["method"] = requestMethod;
        
        // url
        auto url = scheme + "://" + std::string(sourceRequest->hostname) + sourceRequest->unparsed_uri;
        jsonData["url"] = url;
        
        // protocol
        auto serverProtocol = TypesConverter::toStringSafety(sourceRequest->protocol);
        jsonData["protocol"] = serverProtocol;
        
        // characterEncoding
        jsonData["characterEncoding"] = TypesConverter::toStringSafety(sourceRequest->content_encoding);

        // contentLength
        jsonData["contentLength"] = static_cast<Json::Value::UInt>(sourceRequest->clength);

        // contentType
        auto contentType = TypesConverter::toStringSafety(sourceRequest->content_type);
        jsonData["contentType"] = contentType;

        // contextPath
        jsonData["contextPath"] = getContextPath(sourceRequest);
        
        // localAddr
        auto localAddr = TypesConverter::toStringSafety(sourceRequest->connection->local_ip);
        jsonData["localAddr"] = localAddr;

        // localName
        jsonData["localName"] = TypesConverter::toStringSafety(sourceRequest->connection->local_host);
        
        // localPort
        jsonData["localPort"] = sourceRequest->server->port;
        
        // remoteAddr
        auto remoteAddr = TypesConverter::toStringSafety(sourceRequest->useragent_ip);
        jsonData["remoteAddr"] = remoteAddr;
        
        // remoteHost
        auto remoteHost = ap_get_remote_host(sourceRequest->connection, sourceRequest->per_dir_config, REMOTE_NAME, NULL);
        jsonData["remoteHost"] = remoteHost;
        
        // remotePort
        auto remotePort = sourceRequest->useragent_addr->port;
        jsonData["remotePort"] = remotePort;
        
        // secure
        jsonData["secure"] = isSecureProtocol();

        // scheme
        jsonData["scheme"] = scheme;

        // serverName
        jsonData["serverName"] = fqdn;
        
        // serverPort
        auto serverPort = sourceRequest->server->port;
        jsonData["serverPort"] = serverPort;
        
        // servletPath
        jsonData["servletPath"] = std::string();

        auto emptyArray = Json::Value(Json::arrayValue);
        auto contentLanguages = TypesConverter::toVector(sourceRequest->content_languages);
        if (!contentLanguages.empty())
        {
            Json::Value jsonLocales;
            for (auto& contentLanguage : contentLanguages)
                jsonLocales.append(contentLanguage);
            jsonData["locales"] = jsonLocales;
        }
        else jsonData["locales"] = emptyArray;

    }
    std::string GatewayRequest::getScheme(const request_rec* request)
    {
        auto port = getServerPort(request);
        switch (port)
        {
            case 80: 
                return "http";
            case 443: 
                return "https";
            default: 
                return "";
        }
    }
    
    bool GatewayRequest::isSecureProtocol() const
    {
        return (scheme == "https");
    }
    
    std::string GatewayRequest::getContextPath(const request_rec* request) const
    {
        auto url = scheme + "://" + request->hostname;
        auto currentPort = getServerPort(request);
        auto defaultPort = isSecureProtocol() ? 443 : 80;
        if (currentPort != 0 && currentPort != defaultPort)
            url += std::string(StringBuilder() << ':' << currentPort);
        return url;
    }
    
    int GatewayRequest::getServerPort(const request_rec* request)
    {
        auto port = request->server->port;
        if (port == 0)
        {
            auto addresses = request->server->addrs;
            if (addresses)
                // NOTICE: Maybe here we should enumerate all address
                port = addresses->host_port;
        }
        return port;
    }

    const Json::Value& GatewayRequest::getPayload() const
    {
        return jsonData;
    }
}
