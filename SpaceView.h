#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "WindowView.h"
#include "type_info.h"

namespace pk
{
    class Space;

    struct SpaceViewEntry
    {
        std::string _name;
        std::function<void(Space&)> _menu;
        std::function<void(void)> _onClick;

        type _type;
    };

    class SpaceView : WindowView
    {
    public:
        SpaceView(Space& scene);
        void render() override;
        static void add(const std::string& menu, SpaceViewEntry entry);

    private:
        Space& _source;
        static std::map<std::string, SpaceViewEntry> _entries;

        void drawContext();
    };
}