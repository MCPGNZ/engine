#pragma once
#include <string>
#include <filesystem>

#include "FileSystem.h"

namespace pk
{
    class File
    {
    public:
        #pragma region Public methods
        static std::string readAll(const std::filesystem::path& path);
        #pragma endregion
    };
}