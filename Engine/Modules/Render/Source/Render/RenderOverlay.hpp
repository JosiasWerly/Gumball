#pragma once
#ifndef _renderoverlay
#define _renderoverlay

#include "Window.hpp"

#include <string>
using namespace std;

class RenderOverlay {
    friend class RenderModule;

private:
    Window *_window;

public:
    virtual ~RenderOverlay() = default;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;
    virtual void onRender(const double &deltaTime) = 0;
    virtual string name() = 0;
    Window *window() { return _window; }
};
#endif // !_renderoverlay