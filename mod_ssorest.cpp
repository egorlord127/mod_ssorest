#include <httpd.h>
#include <http_request.h>
#include <http_core.h>
#include <http_log.h>
#include "SSORestPlugin.h"
#include "SSORestPluginPool.h"
using namespace ssorest;

static const char* setEnable(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setTraceEnable(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setAccountName(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setGatewayUrl(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setPluginId(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setSecretKey(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setSSOZone(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setIgnoreExt(cmd_parms* command, void* /*config*/, const char* argument);
static const char* setIgnoreUrl(cmd_parms* command, void* /*config*/, const char* argument);
static void registerHooks(apr_pool_t* );


static const command_rec moduleDirectives[] =
{
    AP_INIT_TAKE1
    (
        "SSORestEnabled",            
        reinterpret_cast<cmd_func>(setEnable), 
        NULL, 
        OR_ALL, 
        "Enable or disable mod_ssorest"
    ),

    AP_INIT_TAKE1
    (
        "SSORestTrace",
        reinterpret_cast<cmd_func>(setTraceEnable), 
        NULL, 
        OR_ALL, 
        "Enable or disable libcurl debug"
    ),

    AP_INIT_TAKE1
    (
        "SSORestACOName",        
        reinterpret_cast<cmd_func>(setAccountName), 
        NULL, 
        OR_ALL, 
        ""
    ),
          
    AP_INIT_TAKE1
    (
        "SSORestGatewayUrl",
        reinterpret_cast<cmd_func>(setGatewayUrl), 
        NULL, 
        OR_ALL, 
        "Gateway Location"
    ),

    AP_INIT_TAKE1
    (
        "SSORestPluginId",
        reinterpret_cast<cmd_func>(setPluginId), 
        NULL, 
        OR_ALL, 
        ""
    ),

    AP_INIT_TAKE1
    (
        "SSORestSecretKey",
        reinterpret_cast<cmd_func>(setSecretKey), 
        NULL, 
        OR_ALL,
        ""
    ),

    AP_INIT_ITERATE
    (
        "SSORestSSOZone",
        reinterpret_cast<cmd_func>(setSSOZone), 
        NULL, 
        OR_ALL, 
        ""
    ),
    
    AP_INIT_ITERATE
    (
        "SSORestIgnoreExt",
        reinterpret_cast<cmd_func>(setIgnoreExt), 
        NULL, 
        OR_ALL, 
        ""
    ),

    AP_INIT_ITERATE
    (
        "SSORestIgnoreUrl",
        reinterpret_cast<cmd_func>(setIgnoreUrl), 
        NULL, 
        OR_ALL, 
        ""
    ),
    { NULL }
};

extern "C"
{
    AP_DECLARE_MODULE(ssorest) = {
        STANDARD20_MODULE_STUFF,
        NULL,                   /* Per-directory configuration handler */
        NULL,                   /* Merge handler for per-directory configurations */
        NULL,                   /* Per-server configuration handler */
        NULL,                   /* Merge handler for per-server configurations */
        moduleDirectives, 
        registerHooks 
    };
}

namespace ssorest
{

    static std::shared_ptr<SSORestPlugin> getSSORestPluginFrom(server_rec* server)
    {
        static SSORestPluginPool ssorestpluginPool;
        auto rawVirtualServerName = server->server_hostname;
        if (!rawVirtualServerName)
            throw std::logic_error("ServerName is not set, check virtual server settings");
        std::string virtualServerName(rawVirtualServerName);
        return ssorestpluginPool[virtualServerName];
    }
}

static const char* setEnable(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    if (::strcasecmp(argument, "on") == 0)
        ssorestplugin->setEnable(true);
    else
        ssorestplugin->setEnable(false);
    return NULL;
}
static const char* setTraceEnable(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    if (::strcasecmp(argument, "on") == 0)
        ssorestplugin->setTraceEnable(true);
    else
        ssorestplugin->setTraceEnable(false);
    return NULL;
}
static const char* setAccountName(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->setAccountName(argument);
    return NULL;
}
static const char* setGatewayUrl(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->setGatewayUrl(argument);
    return NULL;
}
static const char* setPluginId(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->setPluginId(argument);
    return NULL;
}
static const char* setSecretKey(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->setSecretKey(argument);
    return NULL;
}
static const char* setSSOZone(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->addSSOZone(argument);
    return NULL;
}

static const char* setIgnoreExt(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->addIgnoreExt(argument);
    return NULL;
}

static const char* setIgnoreUrl(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    ssorestplugin->addIgnoreUrl(argument);
    return NULL;
}

static int processRequest(request_rec* r) {
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)
            "Evalute the Request");
    auto ssorestplugin = getSSORestPluginFrom(r->server);

    bool enabled = ssorestplugin->getEnable();
    bool traceenabled = ssorestplugin->getTraceEnable();
    std::string aconame = ssorestplugin->getAccountName();
    std::string gatewayurl = ssorestplugin->getGatewayUrl();
    std::string pluginid = ssorestplugin->getPluginId();
    std::string secretKey = ssorestplugin->getSecretKey();
    auto ssozone = ssorestplugin->getSSOZone();
    auto ignoreext = ssorestplugin->getIgnoreExt();
    auto ignoreurl = ssorestplugin->getIgnoreUrl();
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"SSORestEnabled:%d", enabled);
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"SSOTraceEnabled:%d", traceenabled);
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"gatewayurl:%s", gatewayurl.c_str());
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"pluginid:%s", pluginid.c_str());
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"secretKey:%s", secretKey.c_str());

    for (std::vector<std::string>::iterator it = ssozone.begin() ; it != ssozone.end(); ++it)
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"ssozone:%s", (*it).c_str());
    
    for (std::vector<std::string>::iterator it = ignoreext.begin() ; it != ignoreext.end(); ++it)
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"ignoreext:%s", (*it).c_str());
    
    for (std::vector<std::string>::iterator it = ignoreurl.begin() ; it != ignoreurl.end(); ++it)
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"ignoreext:%s", (*it).c_str());
    // ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)"ssozone:%s", ssozone.c_str());


    return OK;
}

static void registerHooks(apr_pool_t* ) {
    ::ap_hook_check_access(processRequest, NULL, NULL, APR_HOOK_MIDDLE,
                      AP_AUTH_INTERNAL_PER_URI);
}
