#pragma once
#ifndef __widgetoverlay
#define __widgetoverlay
#include <Render/RenderOverlay.hpp>

//TODO: remove deltatime from render method, add tick methodm rename to UIElement
class GMODULE IWidgetElement {
	friend class WidgetOverlay;
public:
    IWidgetElement() = default;
	~IWidgetElement() = default;
    virtual void render() = 0;
};

class GMODULE WidgetOverlay : public RenderOverlay {
    struct ImGuiIO *guiIO = nullptr;
    std::list<IWidgetElement *> elements;

public:
    ~WidgetOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;
	string name() override { return "widget"; }

    WidgetOverlay &operator<<(IWidgetElement *element);
    WidgetOverlay &operator>>(IWidgetElement *element);
};

#endif // !__widgetoverlay