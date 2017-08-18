#pragma once
#include <string>
#include <map>
#include <httpd.h>
namespace ssorest
{
    class RequestHeaderWrapper{
    private:
        request_rec *request;
    
    public:
        enum class TargetHeader {In, Out};

    public:
        RequestHeaderWrapper(request_rec *r);
        void propagateResponseHeader(const std::map<std::string, std::string>& sourceHeader, TargetHeader targetHeader);
        void propagateCookies(const std::map<std::string, std::string>& sourceCookies, TargetHeader targetHeader);
    };
}