#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "WindowView.h"

namespace pk
{
    class Resources;

    class ResourcesView
    {
    public:
        static void render(Resources& source);

    private:
        static void save(Resources& source);
        static void items(Resources& source);

        enum Mode
        {
            Save,
            Load
        };
    };
}
