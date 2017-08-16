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

        // headers
        auto headers = TypesConverter::toMap(sourceRequest->headers_in);
        Json::Value jsonHeaders;

        // accept-language
        Json::Value jsonHeadersAcceptLanguage = Json::Value(Json::arrayValue);
        jsonHeadersAcceptLanguage.append(headers["Accept-Language"]);
        jsonHeaders["accept-language"] = jsonHeadersAcceptLanguage;
        
        // Cookie
        Json::Value jsonHeadersCookie = Json::Value(Json::arrayValue);
        auto cookies = StringProcessor::split(headers["Cookie"], "; ");
        for (std::vector<std::string>::iterator it = cookies.begin(); it != cookies.end(); ++it)
        {
            jsonHeadersCookie.append((*it));
        }
        jsonHeaders["COOKIE"] = jsonHeadersCookie;

        // connection
        Json::Value jsonHeadersConnection = Json::Value(Json::arrayValue);
        jsonHeadersConnection.append(headers["Connection"]);
        jsonHeaders["connection"] = jsonHeadersConnection;

        // accept
        Json::Value jsonHeadersAccept = Json::Value(Json::arrayValue);
        jsonHeadersAccept.append(headers["Accept"]);
        jsonHeaders["accept"] = jsonHeadersAccept;

        // accept-encoding
        Json::Value jsonHeadersAcceptEncoding = Json::Value(Json::arrayValue);
        jsonHeadersAcceptEncoding.append(headers["Accept-Encoding"]);
        jsonHeaders["accept-encodinge"] = jsonHeadersAcceptEncoding;

        // user-agent
        Json::Value jsonHeadersUserAgent = Json::Value(Json::arrayValue);
        jsonHeadersUserAgent.append(headers["User-Agent"]);
        jsonHeaders["user-agent"] = jsonHeadersUserAgent;

        jsonData["headers"] = jsonHeaders;
        // for (auto& header : headers)
        // {
        //     auto headerName = header.first;
        //     auto& headerValue = header.second;

        //     Logger::emerg(server, "headername:%s", headerName.c_str());
        //     Logger::emerg(server, "headervalue:%s", headerValue.c_str());
        // }
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
