#pragma once
#ifndef __widgetoverlay
#define __widgetoverlay
#include <Render/RenderOverlay.hpp>

//TODO: remove deltatime from render method, add tick methodm rename to UIElement
class GMODULE UIDrawable {
	friend class WidgetOverlay;
public:
    UIDrawable() = default;
	virtual ~UIDrawable() = default;
    virtual void render() = 0;
};

class GMODULE WidgetOverlay : public RenderOverlay {
    struct ImGuiIO *guiIO = nullptr;
    std::list<UIDrawable *> elements;

public:
    unsigned int memory = 0;
    ~WidgetOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;
	string name() override { return "widget"; }

    WidgetOverlay &operator<<(UIDrawable *element);
    WidgetOverlay &operator>>(UIDrawable *element);
};

#endif // !__widgetoverlay