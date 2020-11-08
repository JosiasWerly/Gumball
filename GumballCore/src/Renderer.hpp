#ifndef _renderer
#define _renderer
#include "Gumball.hpp"
#include "GLBuffers.hpp"
#include "Drawcall.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

class ViewMode{
public:
    enum class eProjectionMode{
        Ortho, Perspective, Custom
    };
    eProjectionMode eProjection;
    glm::mat4 mProjection = glm::mat4(1.0f);

    void setProjectionPerspective(float fovy, float aspect, float near = 0.1f, float far = 200.0f){
        mProjection = glm::perspective(fovy, aspect, near, far);
        eProjection = eProjectionMode::Perspective;
    }
    void setProjectionOrtho(float left, float right, float bottom, float top, float near = .1f, float far = 200.0f){
        mProjection = glm::ortho(left, right, bottom, top, near, far);
        eProjection = eProjectionMode::Perspective;
    }
    void customProjection(){
        //projection = false ?//just for testing
       //    glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
       //    glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);
    }
};
class Camera {
    unsigned char renderLayer;
public:    
    ViewMode viewMode;
    Transform transform;

    Camera() {
    }
    unsigned char Layer() {
        return renderLayer;
    }
    unsigned char Layer(unsigned char newLayer) {
        return renderLayer = newLayer;
    }
};
class Renderer {
public:
    int x, y;
    GLFWwindow* window;
    set<iDrawCall*> drawcalls;
    list<Camera*> cameras;

    ~Renderer() {
        glfwTerminate();
    }
    void setup(string winName, int x, int y) {
        this->x = x;
        this->y = y;

        glfwInit();

        //cout << "glVersion " << glGetString(GL_VERSION) << endl;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profile
        gladLoadGL();

        window = glfwCreateWindow(x, y, winName.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        gladLoadGL();

        //viewMode.setProjectionPerspective(glm::radians(45.0f), (float)x / (float)y, .1f, 200.0f);

        glViewport(0, 0, x, y);        
        glEnable(GL_DEPTH_TEST);
        
        //textureAlpha
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //endTextureAlpha
        
        glfwSwapInterval(1);
    }
    
    void drawRender() {
        for (auto& d : drawcalls) {
            drawByLayer(*d);
            //d->draw(*this);
        }
    }
    void clearRender() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void swapBuffers() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Camera* newCamera(unsigned char layer) {
        Camera *newCam = new Camera;
        newCam->viewMode.setProjectionPerspective(glm::radians(45.0f), (float)x / (float)y, .1f, 200.0f);
        newCam->Layer(layer);
        cameras.push_back(newCam);
        return newCam;
    }
    void drawByLayer(iDrawCall &d) {
        for (auto cam : cameras){
            if (cam->Layer() & d.layer) {
                d.sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = cam->viewMode.mProjection;
                d.sa.getParam("uView")->value<Uniform<glm::mat4>>().data = cam->transform.getModel();
                d.draw(*this);
            }
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
        sa.changeShader("default");
        auto& meshData = MeshSystem::instance().getAsset(objName);
        vl.push<float>(3);
        vl.push<float>(3);
        vl.push<float>(2);
        va.bind();
        vb.setData(meshData.mesh.data(), meshData.mesh.size() * sizeof(MeshVertexData));
        ib.setData((unsigned int*)meshData.index.data(), meshData.index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        sa.bind();
        sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = transform.getResultModel();
        //sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;

        sa.uploadParams();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
#endif // !_renderer