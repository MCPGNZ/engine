#pragma once
#include <vector>
#include <string>
#include "Filesystem.h"

namespace pk
{
    class Space
    {
    public:
        friend class SpaceView;

        void add(int id, const std::string& name);
        void remove(int id);

    private:
        std::vector<int> _objects;
        std::vector<std::string> _names;
    };
}
