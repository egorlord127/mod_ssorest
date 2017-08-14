#include "SSORestPlugin.h"

namespace ssorest
{    
    SSORestPlugin::SSORestPlugin() : isEnabled(false)
    {
    }

    void SSORestPlugin::enable()
    {
        isEnabled = true;
    }
    
    void SSORestPlugin::disable()
    {
        isEnabled = false;
    }
    
    void SSORestPlugin::setAccountName(const std::string& value)
    {
        accountName = value;
    }
    
    void SSORestPlugin::setGatewayUrl(const std::string& value)
    {
        gatewayUrl = value;
    }

    void SSORestPlugin::setPluginId(const std::string& value)
    {
        pluginIdKey = value;
    }
    
    void SSORestPlugin::setSecretKey(const std::string& value)
    {
        secretKey = value;
    }

    const bool& SSORestPlugin::getEnable()
    {
        return isEnabled;
    }
    
}
