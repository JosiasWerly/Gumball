#ifndef _widgetoverlay
#define _widgetoverlay
#include "RenderSystem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class WidgetOverlay;
class UIElement;

class WidgetOverlay :
    public IRenderOverlay {
    friend class UIElement;

    ImGuiIO *guiIO = nullptr;
    std::list<UIElement *> elements;
public:

    WidgetOverlay();
    void onAttach() override;
    void onDetach() override;
    void onRender(float deltaTime) override;

    WidgetOverlay &operator<<(UIElement *element) {
        elements.push_back(element);
        return *this;
    }
    WidgetOverlay &operator>>(UIElement *element) {
        elements.remove(element);
        return *this;
    }
};

class UIElement {
    friend class WidgetOverlay;
    bool isVisible = true;
    UIElement *parent = nullptr;
    list<UIElement *> children;
protected:
    void render();
    virtual void begin();
    virtual void end();
    virtual void draw();
public:
    void hide();
    void show();
    void addChild(UIElement *child);
    void delChild(UIElement *child);
    
    Inline bool hasChild(UIElement *child) const { return std::find(children.begin(), children.end(), child) != std::end(children); };
    Inline UIElement *getParent() const { return parent; }
    Inline bool visible() const { return isVisible; }
};

namespace UI {
    
    class Window : 
        public UIElement{
        void begin() override;
        void end() override;

    public:
        string windowName = "Window";
        Window() = default;
    };

    class Text :
        public UIElement {
        void draw() override;

    public:
        string text = "Text";
        
        Text() = default;
    };

    class Button :
        public UIElement {
        bool clicked = false;

        void draw() override {
            ImGui::Button(text.c_str());
            clicked = ImGui::IsItemClicked(0);
        }
    public:
        string text = "Button";

        Button() = default;
        Inline bool isClicked() { return clicked; }
    };

};

#endif // !_widgetoverlay


