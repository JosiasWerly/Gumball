#ifndef _editoroverlay
#define _editoroverlay
#include "RenderSystem.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class EditorOverlay : 
    public IRenderOverlay {
public:
    EditorOverlay() :
        IRenderOverlay("editor") {
    }
    
    void onAttach() override;
    void onDetach() override;
    void onRender(float deltaTime) override;
};


#endif // _editoroverlay
