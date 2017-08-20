#pragma once

#include <string>
#include <sys/stat.h>

namespace ssorest
{
    class File
    {
    private:
        struct stat statBuffer;
    
    public:
        File(const std::string& fileName);
        int getModifiedTime() const;
        static std::string getContent(const std::string& fileName);
    };
}