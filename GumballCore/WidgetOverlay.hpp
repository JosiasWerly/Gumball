#ifndef _widgetoverlay
#define _widgetoverlay
#include "RenderSystem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class WidgetOverlay;
class Widget;

class WidgetOverlay :
    public IRenderOverlay {
    friend class Widget;

    ImGuiIO *guiIO = nullptr;
    std::list<Widget *> elements;
public:

    WidgetOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(const double &deltaTime) override;

    WidgetOverlay &operator<<(Widget *element) {
        elements.push_back(element);
        return *this;
    }
    WidgetOverlay &operator>>(Widget *element) {
        elements.remove(element);
        return *this;
    }
};


//TODO: add a default name to IWidget::Name, since every entity needs a name
//TODO: implement an event based, we're handling by pooling everything...
class Widget { 
    friend class WidgetOverlay;
    bool isVisible = true;
    Widget *parent = nullptr;
    list<Widget *> children;

protected:
    void render();
    virtual void begin();
    virtual void end();
    virtual void draw();

public:
    void hide();
    void show();
    Widget &operator<<(Widget *child);
    Widget &operator>>(Widget *child);
    void addChildren(list<Widget *> children);
    void delChildren(list<Widget *> children);
    
    Inline bool hasChild(Widget *child) const { return std::find(children.begin(), children.end(), child) != std::end(children); };
    Inline Widget *getParent() const { return parent; }
    Inline bool visible() const { return isVisible; }
};

namespace UI {
    class Canvas : public Widget {
    public:
        string windowName = "Window";

        Canvas() = default;
    
    protected:
        void begin() override;
        void end() override;
    };

    class Text : public Widget {
    
    public:
        string text = "Text";        
        Text() = default;

    protected:
        void draw() override;
    };

    class InputText : public Widget {
        char inBuffer[256] = {};
        string str;
    public:
        bool getInput(string &out) const;
    
    protected:
        void draw() override;
    };

    class Button : public Widget {
        bool clicked = false;
    
    public:
        string text = "Button";

        Button() = default;
        Inline bool isClicked() { return clicked; }
    
    protected:
        void draw() override;
    };
};

#endif // !_widgetoverlay