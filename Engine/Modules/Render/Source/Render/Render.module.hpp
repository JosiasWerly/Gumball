#pragma once
#ifndef _rendermodule
#define _rendermodule

#include <Render/Window.hpp>
#include <Render/RenderOverlay.hpp>

#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class SceneOverlay;
class WidgetOverlay;

class GMODULE RenderModule : public ModuleSingleton<RenderModule> {
    list<RenderOverlay*> overlays;
    SceneOverlay *const scene;
    WidgetOverlay *const widget;
    Window mainWindow;

protected:
    bool Load() override;
    void Unload() override;
    void Tick(const double &deltaTime) override;
    EModuleTickType TickType() const override { return EModuleTickType::all; }
    const char* Name() const override { return "Render"; }
public:
    RenderModule();
    ~RenderModule();
    
    void addOverlay(RenderOverlay *overlay, bool front = false);
    void remOverlay(RenderOverlay *overlay);
    RenderOverlay *getOverlay(string name);
    template<class t> t *getOverlay() { 
        for (auto i : overlays) {
            if (t *ti = dynamic_cast<t *>(i)) {
                return ti;
            }
        };
        return nullptr;
    }
    template<class t> t *getOverlay(string name) { return dynamic_cast<t *>(getOverlay(name)); }
    
    Inline list<RenderOverlay *> layerList() const { return overlays; }
    SceneOverlay &getSceneOverlay() { return *scene; }
    WidgetOverlay &getWidgetOverlay() { return *widget; }
    Window &getWindow() { return mainWindow; }    
};
#endif // !_rendermodule