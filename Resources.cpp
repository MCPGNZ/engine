#include "Resources.h"

#include "MessageToken.h"
#include "MessageDispatcher.h"

#include "WindowMessages.h"

#include "logger.h"

#include "json.hpp"
#include "json_bindings.h"
#include "File.h"

#include "stl_extentions.h"

namespace pk
{
    Resources::Resources()
    {
        MessageDispatcher::Register([&](const messages::window::dropped& data){
            _paths.push_back(data.path);
            load(data.path);
        });
    }

    void Resources::load(const std::filesystem::path & path)
    {
        auto& extension = path.extension();
        if(extension == ".png")
        {
            _textures.push_back(Texture{path});
        }
        if(extension == ".json")
        {
            clear();

            auto& contents = File::readAll("data/resources/" + path.string());
            std::vector<std::string> data = json::parse(contents.c_str()).get<std::vector<std::string>>();

            for(auto& entry : data)
            {
                _paths.emplace_back(entry);
                load(*std::last(_paths));
            }
        }
    }
    void Resources::save(const std::filesystem::path & path)
    {
        std::vector<std::string> data;
        for(auto& entry : _paths)
        {
            data.emplace_back(entry.string());
        }

        File::saveAll("data/resources/" + path.string(), json{data}.dump(4));
    }

    void Resources::clear()
    {
        _paths.clear();
        _textures.clear();
    }
}