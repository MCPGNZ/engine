#pragma once
#include <functional>
#include "WindowView.h"
#include "Singleton.h"

namespace pk
{
    class View : public WindowView
    {
    public:
        View(void);

        void render() override;
        std::function<void(void)> _function = nullptr;

        static View* _current;
    };
}
