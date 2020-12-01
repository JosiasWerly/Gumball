#ifndef _renderer
#define _renderer

#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"
#include "Transform.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

class Window {
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

    int x, y;
    GLFWwindow* window;
    FpsCounter fpsCounter;
    
    //string winName;
    //Thread th;
public:
    Window() {

    }
    ~Window() {

    }

    GLFWwindow* getGLFWindow() {
        return window;
    }
    void clearGLStack() {
        glUseProgram(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    float getAspec() {
        return ((float)x / (float)y);
    }
    void create(string winName, int x, int y) {
        this->x = x;
        this->y = y;
        glfwInit();
        //cout << "glVersion " << glGetString(GL_VERSION) << endl;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profile

        window = glfwCreateWindow(x, y, winName.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGL())
            cout << "fail to load window" << endl;

        glViewport(0, 0, x, y);
        glEnable(GL_DEPTH_TEST);
        glfwSwapInterval(1);


        ///////////////////Old stuff//////////////////////
        //textureAlpha
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //endTextureAlpha
    }
    void destroy() {
        glfwTerminate();
    }


    void clearBuffer() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void swapBuffers() {
        fpsCounter.fpsTick();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }

    

    //this will be moved to Renderer class
    double getMS() {
        return fpsCounter.getMsBySec();
    }
};

class iDrawCall {
public:
    Shader sa;
    VertexBuffer vb; // the guy who contains the data
    VertexBufferLayout vl; //the layout of data
    IndexBuffer ib; // data replication
    VertexArray va; //the guys who wrap everything above
    virtual void draw(const class Renderer& renderer) = 0;
};
class Renderer {
    Window* gWindow;
public:
    GLFWwindow* window;
    set<iDrawCall*> drawcalls;
    Camera* camera;
    
    
    Renderer(Window* target) {
        this->gWindow = target;
        this->window = target->getGLFWindow();
    }
    void drawRender() {
        for (auto& d : drawcalls) {
            if (camera) {
                d->sa.params.get<glm::mat4>("uProj") = camera->viewMode.mProjection;
                d->sa.params.get<glm::mat4>("uView") = camera->transform.getModel();
            }            
            d->draw(*this);
            d->sa.params.uploadParams();
        }
    }
    
    Renderer& operator<<(iDrawCall* drawCall) {
        drawcalls.insert(drawCall);
        return *this;
    }
    Renderer& operator>>(iDrawCall* drawCall) {
        auto i = drawcalls.find(drawCall);
        if (i != drawcalls.end())
            drawcalls.erase(i);
    }
};


class Meshdata :
    public iDrawCall {
public:    
    Transform transform;
    Meshdata() {}
    void loadMesh(string objName) {
        auto& am = AssetManager::instance();
        auto meshData = am.getData<MeshData>(objName);
        sa.changeShader("default");
        vl.push<float>(3);
        vl.push<float>(3);
        vl.push<float>(2);
        va.bind();
        vb.setData(meshData.mesh.data(), (unsigned int)meshData.mesh.size() * sizeof(MeshVertexData));
        ib.setData((unsigned int*)meshData.index.data(), (unsigned int)meshData.index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        sa.bind();
        sa.params.get<glm::mat4>("uModel") = transform.getResultModel();        
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
#endif // !_renderer