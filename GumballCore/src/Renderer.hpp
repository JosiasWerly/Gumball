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
    void setSize(int x, int y);

    void clearBuffer();
    void swapBuffers();
    bool shouldClose();
    double getMS();
};


class DrawCall {
public:
    virtual void draw(const class Render& renderer) = 0;
};
class Render {
protected:
    class Window* window;
    struct GLFWwindow* glfWindow;
public:
    set<DrawCall*> drawcalls;
    Camera* camera;
    Render() {}
    Render(Window* window) {
        attachWindow(window);
    }
    void attachWindow(Window* window) {
        this->window = window;
        glfWindow = window->getGLFWindow();
    }

    virtual void diposeRender() {
        for (auto& d : drawcalls) {
            d->draw(*this);
        }
    }
    Render& operator<<(DrawCall* value) {
        drawcalls.insert(value);
        return *this;
    }
    Render& operator>>(DrawCall* value) {
        auto i = drawcalls.find(value);
        if (i != drawcalls.end())
            drawcalls.erase(i);
        return *this;
    }
};
class RenderManager :
    public Singleton<RenderManager> {
public:
    Render* currentContext;
    void disposeRender() {
        currentContext->diposeRender();
    }
};


class Renderer :
    public Render {
public:
    using Render::Render;
    glm::mat4 cameraModelCache;
    void diposeRender() {
        cameraModelCache = camera->transform.getModel();
        for (auto& d : drawcalls) {
            d->draw(*this);
        }
    }
};
class Drawable :
    public DrawCall {
public:
    Shader sa;
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above

    Transform* transform;
    void draw(const class Render& renderer) {
        Renderer& r = (Renderer&)renderer;
        sa.params.get<glm::mat4>("uView") = r.cameraModelCache;
        sa.params.get<glm::mat4>("uProj") = r.camera->viewMode.mProjection;
        sa.params.get<glm::mat4>("uModel") = transform->getResultModel();

        va.bind();
        sa.bind();
        sa.params.uploadParams();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};

#endif // !_renderer