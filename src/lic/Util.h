#pragma once

#include <string>
#include <climits>

class Util
{
public:
    static std::string getFileName(const std::string &path);

    static std::string getBaseName(const std::string& path);

    static std::string getExtension(const std::string& path);

    static std::string getFileNameWithoutExtension(const std::string& path);

    static std::string getRelativePath(const std::string& path, const std::string& base);

    static std::string getAbsolutePath(const std::string& path, const std::string& base);
};