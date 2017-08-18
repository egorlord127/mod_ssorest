#include "SSORestPlugin.h"
#include "GatewayRequest.h"
#include "GatewayResponse.h"
#include "RequestHeaderWrapper.h"
#include "Logger.h"
#include "StringProcessor.h"
#include "Cryptography/Cryptor.h"
#include "Base64.h"
#include "RandomSequence.h"
#include "URI.h"
#include <http_log.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;
namespace ssorest
{    
    SSORestPlugin::SSORestPlugin()
    : isEnabled(false)
    {
        gatewayToken = std::string();        
    }


    bool SSORestPlugin::getEnable()
    {
        return isEnabled;
    }

    bool SSORestPlugin::getTraceEnable()
    {
        return traceEnabled;
    }

    string SSORestPlugin::getAccountName()
    {
        return acoName;
    }

    string SSORestPlugin::getGatewayUrl()
    {
        return gatewayUrl;
    }

    string SSORestPlugin::getGatewayToken()
    {
        return gatewayToken;
    }

    string SSORestPlugin::getPluginId()
    {
        return pluginId;
    }

    string SSORestPlugin::getSecretKey()
    {
        return secretKey;
    }

    vector<std::string> SSORestPlugin::getSSOZone()
    {
        return ssoZone;
    }

    vector<string> SSORestPlugin::getIgnoreExt()
    {
        return ignoreExt;
    }

    vector<string> SSORestPlugin::getIgnoreUrl()
    {
        return ignoreUrl;
    }

    void SSORestPlugin::setEnable(const bool& flag)
    {
        isEnabled = flag;
    }
    void SSORestPlugin::setTraceEnable(const bool& flag)
    {
        traceEnabled = flag;
    }
    void SSORestPlugin::setAccountName(const string& value)
    {
        acoName = value;
    }
    void SSORestPlugin::setGatewayUrl(const string& value)
    {
        gatewayUrl = value;
    }
    void SSORestPlugin::setGatewayToken(const string& value)
    {
        gatewayToken = value;
    }
    void SSORestPlugin::setPluginId(const string& value)
    {
        pluginId = value;
    }
    void SSORestPlugin::setSecretKey(const string& value)
    {
        secretKey = value;
    }
    void SSORestPlugin::addSSOZone(const string& value)
    {
        ssoZone.push_back(value);
    }

    void SSORestPlugin::addIgnoreExt(const string& value)
    {
        ignoreExt.push_back(value);
    }

    void SSORestPlugin::addIgnoreUrl(const string& value)
    {
        ignoreUrl.push_back(value);
    }  

    int SSORestPlugin::process(request_rec* r)
    {
        sourceRequest = r;
        if(!isEnabled)
        {
            Logger::notice(r, "SSO/Rest Plugin is disabled");
            return DECLINED;
        }
        
        Logger::notice(r, "Processing new request");

        
        if (r->uri)
        {
            /* 1.Check if the request uri matches with ignored extension */
            std::string filename(r->uri);
            std::string fileext(filename.substr(filename.find_last_of(".") + 1));
            if (!fileext.empty() && filename != fileext)
            {
                if (std::find(ignoreExt.begin(), ignoreExt.end(), fileext) != ignoreExt.end())
                {
                    Logger::notice(r, "Ignore Extension Matched");
                    return OK;
                }
            }

            /* 2.Check if the request uri matches with ignored url */
            for (auto it = ignoreUrl.begin() ; it != ignoreUrl.end(); ++it)
            {
                std::string uri(r->uri);
                if (uri.find(*it) != std::string::npos)
                {
                    Logger::notice(r, "Ignore Url Matched");
                    return OK;
                }
            }

            if(gatewayUrl.empty())
            {
                Logger::notice(r, "No SSORestGatewayUrl in configuration");
                return (HTTP_FORBIDDEN);
            }
            GatewayRequest gatewayRequest(r);
            gatewayRequest.set(GatewayRequest::acoName, acoName);
            gatewayRequest.set(GatewayRequest::pluginId, pluginId);
            gatewayRequest.set(GatewayRequest::gatewayToken, gatewayToken);
            gatewayRequest.buildJsonRequest(ssoZone);

            Logger::styledDebug(r, "Sending JSon request to Gateway:" + gatewayRequest.getPayload().toStyledString());
            
            GatewayResponse response(gatewayRequest.sendTo(gatewayUrl, traceEnabled));
            auto jsonResponse = response.getJsonResponse();
            auto responseHeaders = response.getResponseHeader();
            Logger::styledDebug(r, "Parsed reply from Gateway: " + jsonResponse.toStyledString());

            auto status = response.getJsonResponseStatus();
            if (status == HTTP_CONTINUE)
            {
                RequestHeaderWrapper requestHeaderWrapper(r);
                requestHeaderWrapper.propagateResponseHeader(responseHeaders, RequestHeaderWrapper::TargetHeader::In);
                
                auto additionalCookies = response.getResponseCookies();
                requestHeaderWrapper.propagateCookies(additionalCookies, RequestHeaderWrapper::TargetHeader::Out);

                return (DECLINED);
            }
            else if (status == HTTP_NOT_EXTENDED)
            {
                auto responseBody = response.getResponseBody();
                if (responseBody.find("Signature Needed") != std::string::npos)
                {
                    auto randomText = RandomSequence::generate(32);
                    Logger::notice(r, "Generated randomText: %s", randomText.c_str());
                    Cryptor::HMACSHA1Digest digest;
                    Cryptor::computeHMACSHA1(randomText, secretKey, digest);
                    auto signedRandomText = base64_encode(digest.data(), static_cast<unsigned int>(digest.size()));
                    Logger::notice(r, "Generated HMAC: %s", signedRandomText.c_str());
                    
                    std::string encodedSignedRandomText;
                    URI::encode(signedRandomText, encodedSignedRandomText);
                    
                    gatewayRequest.set(GatewayRequest::randomText, randomText);
                    gatewayRequest.set(GatewayRequest::randomTextSigned, encodedSignedRandomText);
                }
                else
                {
                }
                return process(r);
            }
            else
            {
                RequestHeaderWrapper requestHeaderWrapper(r);
                requestHeaderWrapper.propagateResponseHeader(responseHeaders, RequestHeaderWrapper::TargetHeader::Out);
                
                auto additionalCookies = response.getResponseCookies();
                requestHeaderWrapper.propagateCookies(additionalCookies, RequestHeaderWrapper::TargetHeader::Out);
                
                // if (gatewayResponse.isSubresponseBodySet())
                // {
                //     auto moduleResponse = base64_decode(gatewayResponse.getSubresponseBody());
                //     ap_rwrite(moduleResponse.c_str(), static_cast<int>(moduleResponse.size()), incomingRequest);
                // }
                return status;
            }
        }
        

        return OK;
    }

}
