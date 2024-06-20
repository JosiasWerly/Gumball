#pragma once
#ifndef __widgetoverlay
#define __widgetoverlay
#include <Render/RenderOverlay.hpp>
#include <Render/Widget/Widget.hpp>

class WidgetOverlay;
class Widget;

class GMODULE WidgetOverlay : public RenderOverlay {
    friend class Widget;

    struct ImGuiIO *guiIO = nullptr;
    std::list<Widget *> elements;

public:
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;
	string name() override { return "widget"; }

    WidgetOverlay &operator<<(Widget *element);
    WidgetOverlay &operator>>(Widget *element);
};

#endif // !__widgetoverlay