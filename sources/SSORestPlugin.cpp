#include "SSORestPlugin.h"
#include "GatewayRequest.h"
#include "GatewayResponse.h"
#include "Logger.h"
#include "NetworkInfo.h"
#include "StringProcessor.h"

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
    , fqdn(NetworkInfo().getFQDN())
    {
        
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
            gatewayRequest.setPluginId(pluginId);
            gatewayRequest.setAcoName(acoName);

            GatewayResponse response(gatewayRequest.sendTo(gatewayUrl));
            auto jsonResponse = response.getJsonResponse();
            Logger::styledDebug(r, jsonResponse.toStyledString());
        }
        

        return OK;
    }

}
