#include <algorithm>
#include <curl/curl.h>
#include <memory>
#include <vector>

#include "CurlWrapper.h"

namespace ssorest
{       
    CurlWrapper::CurlWrapper() : curl(nullptr)
    {
        curl = ::curl_easy_init();
    }
    
    CurlWrapper::~CurlWrapper()
    {
        ::curl_easy_cleanup(curl);
    }
    
    CurlWrapper::operator CURL*()
    {
        return curl;
    }
    
    void CurlWrapper::verifyResult(CURLcode curlCode)
    {
    }
    
    size_t CurlWrapper::writeCallback(void* newData, size_t elementsQuantity, size_t elementSize, WriteBuffer* outputBuffer)
    {
        auto newDataSize = elementsQuantity * elementSize;
        auto newDataBytes = reinterpret_cast<const char*>(newData);
        outputBuffer->insert(outputBuffer->end(), newDataBytes, newDataBytes + newDataSize);
        return newDataSize;
    }
    
    std::string CurlWrapper::getUrlEncodedVersionOf(const std::string& plainText)
    {
        auto rawEncodedText = ::curl_easy_escape(curl, plainText.c_str(), plainText.size());
        std::string encodedText(rawEncodedText);
        ::curl_free(rawEncodedText);
        return encodedText;
    }
}
