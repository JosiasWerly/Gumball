#ifndef _renderer
#define _renderer

#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Window.hpp"



//i`m just too tired and too hungry todo this better//

class IDrawCall {
    bool isEnableDraw;
    unsigned int drawLayer;
public:
    Shader sa; //the shader...
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above
    
    Transform* transform;
    
    bool getEnableDraw() { return this->isEnableDraw; }
    unsigned int getDrawLayer() { return drawLayer; }
    
    virtual void setEnableDraw(bool enable) { this->isEnableDraw = enable; }
    virtual void setDrawLayer(unsigned int newDrawLayer) { drawLayer = newDrawLayer; }
    virtual void draw(const class IRenderDisposer& renderer) = 0;
};
class IRenderDisposer {
public:
    class IRender* renderRef;
    glm::mat4 cameraModelCache;
    IRenderDisposer(class IRender* renderRef) : 
        renderRef(renderRef){
    }
    virtual void disposeRender();
};
class IRender {
    friend class IRenderDisposer;
    IRenderDisposer* diposeRender;
    set<Camera*> cameras;
    set<IDrawCall*> drawcalls;    
public:
    IRender() {
        setRenderDisposer<>();
    }
    void drawRender() {
        diposeRender->disposeRender();
    }

    template<typename T>T* getRenderDisposer() { 
        return (T*)diposeRender; 
    }
    template<typename T = IRenderDisposer>void setRenderDisposer() {
        if (diposeRender)
            delete diposeRender;
        diposeRender = new T(this);
    }

    IRender& operator<<(IDrawCall* value) {
        drawcalls.insert(value);
        return *this;
    }
    IRender& operator>>(IDrawCall* value) {
        auto i = drawcalls.find(value);
        if (i != drawcalls.end())
            drawcalls.erase(i);
        return *this;
    }
    IRender& operator<<(Camera* value) {
        cameras.insert(value);
        return *this;
    }
    IRender& operator>>(Camera* value) {
        auto i = cameras.find(value);
        if (i != cameras.end())
            cameras.erase(i);
        return *this;
    }
};
class RenderManager :
    public Singleton<RenderManager> {
public:
    IRender render[4];
    RenderManager() {
    }
    void disposeRender() {
        for (auto& r : render)
            r.drawRender();
    }
    RenderManager& operator<<(IDrawCall* value) {        
        for (int x=0; x < 4; x++) {
            IRender& r = render[x];
            auto l = value->getDrawLayer();
            if (x == l || x & l)
                r << value;
        }
        return *this;
    }
    RenderManager& operator>>(IDrawCall* value) {
        for (int x = 0; x < 4; x++) {
            IRender& r = render[x];
            auto l = value->getDrawLayer();
            if (x == l || x & l)
                r >> value;
        }
        return *this;
    }
};

class Drawable :
    public IDrawCall {
public:
    Drawable() {
        setEnableDraw(true);
    }
    void setDrawLayer(unsigned int layer) {
        auto& r = RenderManager::instance();
        r >> this;
        IDrawCall::setDrawLayer(layer);
        r << this;
    }
    void setEnableDraw(bool enable) {
        IDrawCall::setEnableDraw(enable);
        auto& r = RenderManager::instance();
        if(enable)
            r << this;
        else 
            r >> this;
    }
    void draw(const class IRenderDisposer& renderer) {
        va.bind();
        sa.bind();
        sa.params.uploadParams();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
#endif // !_renderer