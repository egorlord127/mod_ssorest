#include "GatewayRequest.h"
#include "TypesConverter.h"
#include "StringBuilder.h"
#include "StringProcessor.h"
#include "Logger.h"

#include <http_request.h>
#include <http_core.h>
namespace ssorest
{
    GatewayRequest::GatewayRequest(request_rec* sourceRequest, const std::string& fqdn)
    : server(sourceRequest->server)
    , scheme(getScheme(sourceRequest))
    {
        // TODO: 
        // method
        jsonData["method"] = TypesConverter::toStringSafety(sourceRequest->method);
        
        // url
        jsonData["url"] = scheme + "://" + TypesConverter::toStringSafety(sourceRequest->hostname) + TypesConverter::toStringSafety(sourceRequest->unparsed_uri);
        
        // protocol
        jsonData["protocol"] = TypesConverter::toStringSafety(sourceRequest->protocol);
        
        // characterEncoding
        jsonData["characterEncoding"] = TypesConverter::toStringSafety(sourceRequest->content_encoding);

        // contentLength
        jsonData["contentLength"] = static_cast<Json::Value::UInt>(sourceRequest->clength);

        // contentType
        jsonData["contentType"] = TypesConverter::toStringSafety(sourceRequest->content_type);

        // contextPath
        jsonData["contextPath"] = TypesConverter::toStringSafety(ap_document_root(sourceRequest));
        
        // localAddr
        jsonData["localAddr"] = TypesConverter::toStringSafety(sourceRequest->connection->local_ip);

        // localName
        jsonData["localName"] = TypesConverter::toStringSafety(sourceRequest->server->server_hostname);
        
        // localPort
        jsonData["localPort"] = static_cast<Json::Value::UInt>(getServerPort(sourceRequest));
        
        // remoteAddr
        jsonData["remoteAddr"] = TypesConverter::toStringSafety(sourceRequest->useragent_ip);
        
        // remoteHost
        jsonData["remoteHost"] = TypesConverter::toStringSafety(sourceRequest->useragent_ip);
        
        // remotePort
        jsonData["remotePort"] = static_cast<Json::Value::UInt>(sourceRequest->useragent_addr->port);
        
        // secure
        jsonData["secure"] = isSecureProtocol();

        // scheme
        jsonData["scheme"] = scheme;

        // serverName
        jsonData["serverName"] = TypesConverter::toStringSafety(sourceRequest->server->server_hostname);
        
        // serverPort
        jsonData["serverPort"] = static_cast<Json::Value::UInt>(getServerPort(sourceRequest));
        
        // servletPath
        jsonData["servletPath"] = std::string();

        // TODO: locale

        // headers Array
        auto headers = TypesConverter::toMap(sourceRequest->headers_in);
        Json::Value jsonHeaders;

        // accept-language
        Json::Value jsonHeaderAcceptLanguage = Json::Value(Json::arrayValue);
        jsonHeaderAcceptLanguage.append(headers["Accept-Language"]);
        jsonHeaders["accept-language"] = jsonHeaderAcceptLanguage;
        
        // Cookie
        Json::Value jsonHeaderCookie = Json::Value(Json::arrayValue);
        auto cookies = StringProcessor::split(headers["Cookie"], "; ");
        for (std::vector<std::string>::iterator it = cookies.begin(); it != cookies.end(); ++it)
        {
            jsonHeaderCookie.append((*it));
        }
        jsonHeaders["COOKIE"] = jsonHeaderCookie;

        // connection
        Json::Value jsonHeaderConnection = Json::Value(Json::arrayValue);
        jsonHeaderConnection.append(headers["Connection"]);
        jsonHeaders["connection"] = jsonHeaderConnection;

        // accept
        Json::Value jsonHeaderAccept = Json::Value(Json::arrayValue);
        jsonHeaderAccept.append(headers["Accept"]);
        jsonHeaders["accept"] = jsonHeaderAccept;

        // host
        Json::Value jsonHeaderHost = Json::Value(Json::arrayValue);
        jsonHeaderHost.append(headers["Host"]);
        jsonHeaders["host"] = jsonHeaderHost;

        // accept-encoding
        Json::Value jsonHeaderAcceptEncoding = Json::Value(Json::arrayValue);
        jsonHeaderAcceptEncoding.append(headers["Accept-Encoding"]);
        jsonHeaders["accept-encoding"] = jsonHeaderAcceptEncoding;

        // user-agent
        Json::Value jsonHeaderUserAgent = Json::Value(Json::arrayValue);
        jsonHeaderUserAgent.append(headers["User-Agent"]);
        jsonHeaders["user-agent"] = jsonHeaderUserAgent;

        jsonData["headers"] = jsonHeaders;

        // cookies Array
        Json::Value javaCookies;
        auto unparsedPairs = StringProcessor::split(headers["Cookie"], ";");
        for (const auto& unparsedPair : unparsedPairs)
        {
            auto cookieKeyValue = StringProcessor::split(unparsedPair, "=");
            if (cookieKeyValue.size() == 2)
            {
                Json::Value javaCookie;
                javaCookie["name"] = StringProcessor::trimmed(cookieKeyValue[0]);
                javaCookie["value"] = StringProcessor::trimmed(cookieKeyValue[1]);
                javaCookies.append(javaCookie);
            }
        }
        jsonData["cookies"] = javaCookies;

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
