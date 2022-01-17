#ifndef _editoroverlay
#define _editoroverlay
#include "RenderSystem.hpp"

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
