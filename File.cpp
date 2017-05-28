#pragma once
#include <fstream>
#include "File.h"

namespace pk
{
    std::string File::readAll(const std::filesystem::path& path)
    {
        std::string result;
        std::ifstream file{path, std::ios::in | std::ios::binary};

        file.seekg(0, std::ios::end);
        result.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&result[0], result.size());
        file.close();

        return result;
    }
    void File::saveAll(const std::filesystem::path & path, const std::string & data)
    {
        std::ofstream file{path};
        file << data;
        file.close();
    }
}