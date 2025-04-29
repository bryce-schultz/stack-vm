//***********************************************
// Util.cpp
//***********************************************

#include <fstream>

#include "Util.h"

std::string Util::getFileName(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        return path;
    }
    return path.substr(pos + 1);
}

std::string Util::getBaseName(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        return path;
    }
    return path.substr(0, pos);
}

std::string Util::getExtension(const std::string &path)
{
    size_t pos = path.find_last_of(".");
    if (pos == std::string::npos)
    {
        return "";
    }
    return path.substr(pos + 1);
}

std::string Util::getFileNameWithoutExtension(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    size_t pos2 = path.find_last_of(".");
    if (pos == std::string::npos)
    {
        if (pos2 == std::string::npos)
        {
            return path;
        }

        return path.substr(0, pos2);
    }

    return path.substr(pos + 1, pos2 - pos - 1);
}

std::string Util::getRelativePath(const std::string &path, const std::string &base)
{
    size_t pos = base.find_last_of("/\\");
    return path.substr(pos + 1);
}

std::string Util::getAbsolutePath(const std::string &path, const std::string &base)
{
    size_t pos = base.find_last_of("/\\");
    return path.substr(pos + 1);
}

std::string Util::readWholeFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return "";
    }

    std::string result;
    std::string line;
    while (std::getline(file, line))
    {
        result += line + '\n';
    }

    file.close();
    return result;
}
