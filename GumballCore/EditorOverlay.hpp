#ifndef _editoroverlay
#define _editoroverlay
#include "RenderSystem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class EditorOverlay;
class UIElement;

class EditorOverlay : 
    public IRenderOverlay {
    ImGuiIO *guiIO = nullptr;
    std::list<UIElement *> elements;
public:
    //crap but it'll help me debug stuff
    std::unordered_map<string, double> msStats;
    

    EditorOverlay();    
    void onAttach() override;
    void onDetach() override;
    void onRender(float deltaTime) override;
};

class UIElement {
    friend class EditorOverlay;

    UIElement *parent = nullptr;
    list<UIElement *> nodes;
protected:
    void render() {
        begin();
        draw();
        for (auto n : nodes)
            n->render();
        end();
    }
    virtual void begin() {}
    virtual void end() {}
    virtual void draw() {}
public:
    void hide();
    void show();
};

namespace UI {
    
    class Window : 
        public UIElement{
        void begin() override;
        void end() override;

    public:
        Window() = default;
    };

    class Text :
        public UIElement {
        void draw() override;

    public:
        string text;
        
        Text() = default;
    };

};

#endif // _editoroverlay


