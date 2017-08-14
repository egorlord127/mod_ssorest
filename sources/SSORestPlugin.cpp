#include "SSORestPlugin.h"
#include <http_log.h>
using namespace std;
namespace ssorest
{    
    SSORestPlugin::SSORestPlugin()
    : isEnabled(false)
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
}
