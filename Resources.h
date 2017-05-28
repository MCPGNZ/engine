#pragma once
#include <vector>
#include <string>

#include "Filesystem.h"
#include "Texture.h"
#include "Data.h"

namespace pk
{
    class Resources : public Data<Resources>
    {
    public:
        Resources();

        void load(const std::filesystem::path& path);
        void save(const std::filesystem::path& path);

        void clear();

        std::vector<pk::Texture> _textures;

    private:
        std::vector<std::filesystem::path> _paths;

        friend class ResourcesView;
    };
}
