#ifndef _renderer
#define _renderer

#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Input.hpp"



//i`m just too tired and too hungry todo this better//


class FpsCounter {
        double lastTime = 0;
        unsigned int frames = 0;
    public:
        double ms = 0;
        FpsCounter() {
            lastTime = glfwGetTime();
        }
        void fpsTick() {
            double actualTime = glfwGetTime();
            frames++;
            if (actualTime - lastTime >= 1.0) {
                ms = 1000 / frames;
                frames = 0;
                lastTime += 1.0f;
            }
        }
        double getMsBySec() {
            return ms;
        }
};




class Window {
    int x, y;
    GLFWwindow* window;
    FpsCounter fpsCounter;
public:
    Window();
    ~Window();

    GLFWwindow* getGLFWindow();
    void clearGLStack();
    float getAspec();
    void create(string winName, int x, int y);
    void config(string winName, int x, int y);
    void setSize(int x, int y);

    void clearBuffer();
    void swapBuffers();
    bool shouldClose();
    double getMS();
};


class IDrawCall {
public:
    virtual void draw(const class IRender& renderer) = 0;
};
class IRender {
protected:
    class Window* window;
    struct GLFWwindow* glfWindow;
public:
    Camera* camera;
    set<IDrawCall*> drawcalls;
    
    glm::mat4 cameraModelCache;
    virtual void diposeRender() {
        cameraModelCache = camera->transform.getModel();
        for (auto& d : drawcalls)
            d->draw(*this);
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

    void attachCamera(Camera* camera) {
        this->camera = camera;
    }
    void attachWindow(Window* window) {
        this->window = window;
        glfWindow = window->getGLFWindow();
    }
};

class RenderManager :
    public Singleton<RenderManager> {
public:
    set<IDrawCall*> drawcalls;
    set<IRender*> renderers;
    RenderManager() {

    }
    void disposeRender() {
        for (auto r : renderers)
            r->diposeRender();
    }

    void changeDrawLayer(IRender* obj, unsigned int newLayer) {
        
    }
    void changeDrawLayer(IDrawCall* obj, unsigned int newLayer) {

    }
    RenderManager& operator<<(IDrawCall* value) {
        drawcalls.insert(value);
        return *this;
    }
    RenderManager& operator>>(IDrawCall* value) {
        auto i = drawcalls.find(value);
        if (i != drawcalls.end())
            drawcalls.erase(i);
        return *this;
    }
    
    RenderManager& operator<<(IRender* value) {
        renderers.insert(value);
        return *this;
    }
    RenderManager& operator>>(IRender* value) {
        auto i = renderers.find(value);
        if (i != renderers.end())
            renderers.erase(i);
        return *this;
    }
};


class Render : 
    public IRender {
protected:
    void setDrawLayer(unsigned int layer) {
        auto& r = RenderManager::instance();
        r.changeDrawLayer(this, layer);
    }
    void setEnableDraw(bool enable) {
        auto& r = RenderManager::instance();
        if (enable)
            r << this;
        else
            r >> this;
    }
};

class Drawable :
    public IDrawCall {
public:
    Shader sa;
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above

    Transform* transform;

    void setDrawLayer(unsigned int layer) {
        auto& r = RenderManager::instance();
        r.changeDrawLayer(this, layer);
    }
    void setEnableDraw(bool enable) {
        auto& r = RenderManager::instance();
        if(enable)
            r << this;
        else 
            r >> this;
    }
    void draw(const class IRender& renderer) {        
        sa.params.get<glm::mat4>("uView") = renderer.cameraModelCache;
        sa.params.get<glm::mat4>("uProj") = renderer.camera->viewMode.mProjection;
        sa.params.get<glm::mat4>("uModel") = transform->getResultModel();

        va.bind();
        sa.bind();
        sa.params.uploadParams();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};

#endif // !_renderer