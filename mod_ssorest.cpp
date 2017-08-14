#include <httpd.h>
#include <http_request.h>
#include <http_core.h>
#include <http_log.h>
#include "SSORestPlugin.h"
#include "SSORestPluginPool.h"
using namespace ssorest;

static const char* setEnabled(cmd_parms* command, void* /*config*/, const char* argument);
static void registerHooks(apr_pool_t* );


static const command_rec moduleDirectives[] =
{
    AP_INIT_TAKE1("SSORestEnabled",            reinterpret_cast<cmd_func>(setEnabled), NULL, OR_ALL, 
        "Enable or disable mod_idfc"),
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

static bool isOn(const char* value)
{
    return (::strcasecmp(value, "on") == 0);
}

static const char* setEnabled(cmd_parms* command, void* /*config*/, const char* argument)
{
    auto ssorestplugin = getSSORestPluginFrom(command->server);
    if (isOn(argument))
        ssorestplugin->enable();
    else
        ssorestplugin->disable();
    return NULL;
}

static int processRequest(request_rec* r) {
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)
            "Evalute the Request");
    auto ssorestplugin = getSSORestPluginFrom(r->server);
    bool enabled = ssorestplugin->getEnable();
    if(enabled)
    {
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)
            "Enabled");
    }
    else 
    {
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)
            "Disabled");
    }
    return OK;
}

static void registerHooks(apr_pool_t* ) {
    ::ap_hook_check_access(processRequest, NULL, NULL, APR_HOOK_MIDDLE,
                      AP_AUTH_INTERNAL_PER_URI);
}