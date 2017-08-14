#include <httpd.h>
#include <http_request.h>
#include <http_core.h>
#include <http_log.h>

static void registerHooks(apr_pool_t* );

extern "C"
{
    AP_DECLARE_MODULE(ssorest) = {
        STANDARD20_MODULE_STUFF,
        NULL,                   /* Per-directory configuration handler */
        NULL,                   /* Merge handler for per-directory configurations */
        NULL,                   /* Per-server configuration handler */
        NULL,                   /* Merge handler for per-server configurations */
        NULL, 
        registerHooks 
    };
}

static int processRequest(request_rec* r) {
    ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, APLOGNO(10000)
            "Evalute the Request");
    return OK;
}

static void registerHooks(apr_pool_t* ) {
    ::ap_hook_check_access(processRequest, NULL, NULL, APR_HOOK_MIDDLE,
                      AP_AUTH_INTERNAL_PER_URI);
}