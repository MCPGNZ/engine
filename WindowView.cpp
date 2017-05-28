#include "WindowView.h"
#include <algorithm>

namespace pk
{
    std::vector<WindowView*> WindowView::_views;

    WindowView::WindowView()
    {
        _views.push_back(this);
    }

    WindowView::~WindowView()
    {
        _views.erase(std::find(_views.cbegin(), _views.cend(), this));
    }
}