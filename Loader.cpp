#include "Loader.h"
#include "File.h"
#include "json.hpp"
#include "stl_extentions.h"
#include "MessageDispatcher.h"

namespace pk
{
    void Loader::load(const std::filesystem::path& path)
    {
        if(path.extension() == ".json")
        {
            auto& data = File::readAll(path);
            json j = json::parse(data);

            // special iterator member functions for objects
            for(json::iterator it = j.begin(); it != j.end(); ++it)
            {
                auto key = it.key();
                auto value = it.value();

                if(std::contains(_loaders, key))
                {
                    _loaders[key](value);
                }
            }
        }
    }

    void Loader::add(const std::string & type, void(*loader)(json &))
    {
        _loaders[type] = loader;
    }
}