#pragma once
#include <string>
#include <unordered_map>

#include <map>
#include <type_traits>
#include <functional>

#include "MessageToken.h"
#include "Filesystem.h"


#include "json.hpp"


namespace pk
{
    class Loader
    {
    public:
        void load(const std::filesystem::path& path);
        void add(const std::string& type, void(*loader)(json&));

    private:
        std::map<std::string, std::function<void(json&)>> _loaders;
    };
}
