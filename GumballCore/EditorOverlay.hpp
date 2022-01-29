#ifndef _editoroverlay
#define _editoroverlay
#include "RenderSystem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class EditorOverlay : 
    public IRenderOverlay {
    ImGuiIO *guiIO = nullptr;
public:
    //crap but it'll help me debug stuff
    std::unordered_map<string, double> msStats;
    

    EditorOverlay() :
        IRenderOverlay("editor") {
    }
    
    void onAttach() override;
    void onDetach() override;
    void onRender(float deltaTime) override;
};

#endif // _editoroverlay


