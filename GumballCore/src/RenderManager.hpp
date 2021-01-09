#ifndef _renderManager
#define _renderManager

#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Window.hpp"



//i'm just too tired and too hungry todo this better

class IRenderBoard{
    //i rather would use composition or association here, but i'm only 1 guy 
    //and with my vacation ending, so this should do the trick.
    bool isEnableDraw;
    unsigned int drawLayer;
public:
    bool getEnableDraw() { return this->isEnableDraw; }
    unsigned int getDrawLayer() { return drawLayer; }
    virtual void setEnableDraw(bool enable) { this->isEnableDraw = enable; }
    virtual void setDrawLayer(unsigned int newDrawLayer) { drawLayer = newDrawLayer; }
};
class IView : 
    public IRenderBoard {
public:
    class ViewMode {
    public:
        //projection = false ?//just for testing
        //glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
        //glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);

        enum class eProjectionMode {
            Ortho, Perspective, Custom
        };
        eProjectionMode eProjection;
        glm::mat4 mProjection = glm::mat4(1.0f);

        void setProjectionPerspective(float fovy = glm::radians(45.0f), float aspect = 1.3333333730697632, float zNear = 0.1f, float zFar = 500.0f) {
            mProjection = glm::perspective(fovy, aspect, zNear, zFar);
            eProjection = eProjectionMode::Perspective;
        }
        void setProjectionOrtho(float left, float right, float bottom, float top, float zNear = .1f, float zFar = 200.0f) {
            mProjection = glm::ortho(left, right, bottom, top, zNear, zFar);
            eProjection = eProjectionMode::Perspective;
        }
        void setProjectionCustom(glm::mat4 customProj) {
            mProjection = customProj;
            eProjection = eProjectionMode::Custom;
        }
        const glm::mat4& getProjection() {
            return mProjection;
        }
    };
    Transform transform;
    ViewMode viewMode;
    IView() {
        viewMode.setProjectionPerspective();
    }
};
class IDrawCall :
    public IRenderBoard {
public:
    Shader sa; //the shader...
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above
    Transform* transform;
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
    set<IView*> views;
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
    IRender& operator<<(IView* value) {
        views.insert(value);
        return *this;
    }
    IRender& operator>>(IView* value) {
        auto i = views.find(value);
        if (i != views.end())
            views.erase(i);
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

    // ** warning ** bad engineering ahead. REAL BAD.
    //i think this could be a template method, leaving the resolution of type to runtime.
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
    RenderManager& operator<<(IView* value) {
        for (int x = 0; x < 4; x++) {
            IRender& r = render[x];
            auto l = value->getDrawLayer();
            if (x == l || x & l)
                r << value;
        }
        return *this;
    }
    RenderManager& operator>>(IView* value) {
        for (int x = 0; x < 4; x++) {
            IRender& r = render[x];
            auto l = value->getDrawLayer();
            if (x == l || x & l)
                r >> value;
        }
        return *this;
    }
    // you are safe for now 
    // but this would increase the complexity of the code... i have to sleep on this some good ideia might come.
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
class Viewport :
    public IView {
public:
    Viewport() {
        setEnableDraw(true);
    }
    void setDrawLayer(unsigned int layer) {
        auto& r = RenderManager::instance();
        r >> this;
        IView::setDrawLayer(layer);
        r << this;
    }
    void setEnableDraw(bool enable) {
        IView::setEnableDraw(enable);
        auto& r = RenderManager::instance();
        if (enable)
            r << this;
        else
            r >> this;
    }
};
#endif // !_renderManager