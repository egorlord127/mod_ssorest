#include "File.h"
#include <fstream>
#include <streambuf>
namespace ssorest
{
    File::File(const std::string& fileName)
    {
        if (::stat(fileName.c_str(), &statBuffer) != 0)
        {
            // TODO: Error Handler
        }
    }
    int File::getModifiedTime() const
    {
        return statBuffer.st_mtime;
    }

    std::string File::getContent(const std::string& fileName)
    {
        std::ifstream fileStream(fileName.c_str());
        return std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    }
}