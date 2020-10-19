#include "Gumball.hpp"
#include "GLBuffers.hpp"
#include "Drawcall.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"


#include "stb_image.h"
struct TextureReference {
    unsigned int glBufferId;
    unsigned char* memoryBuffer;
    int width, height;
};
class TextureSystem :
    public AssetFactory<TextureReference>,
    public Singleton<TextureSystem> {
public:
    void loadFromFile(string filePath) {
        stbi_set_flip_vertically_on_load(true);
        int x, y, channels;
        unsigned char* imageBuffer = stbi_load(filePath.c_str(), &x, &y, &channels, 4);
        unsigned int bufferId = 0;

        if (imageBuffer) {
            glDCall(glGenTextures(1, &bufferId));
            glDCall(glBindTexture(GL_TEXTURE_2D, bufferId));
            //filtering
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            //wrapping
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            glDCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

            glDCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer));
            glDCall(glBindTexture(GL_TEXTURE_2D, 0));
            assets.push(gbLib::getNameOfFilePath(filePath), { bufferId, imageBuffer, x, y });
        }
        else throw;
    }
    void unload(string name) {
        auto s = assets.get(name);
        glDeleteTextures(1, &s->glBufferId);
        delete s->memoryBuffer;
        assets.pop(name);
    }
    void unloadAll() {

    }
};
class Texture {
public:
    TextureReference textureData;
    Texture() {
    }
    Texture(string name){
        changeTexture(name);
    }
    void changeTexture(string name) {
        textureData = TextureSystem::instance().getAsset(name);
    }
    void bind(char slot = 0) {
        glDCall(glActiveTexture(GL_TEXTURE0 + slot));
        glDCall(glBindTexture(GL_TEXTURE_2D, textureData.glBufferId));
    }
    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};


class ViewMode{
public:
    enum class eProjectionMode{
        Ortho, Perspective, Custom
    };
    eProjectionMode eProjection;
    glm::mat4 
        mView = glm::mat4(1.0f),
        mProjection = glm::mat4(1.0f);

    void setProjectionPerspective(float fovy, float aspect, float near = 0.1f, float far = 200.0f){
        mProjection = glm::perspective(fovy, aspect, near, far);
        eProjection = eProjectionMode::Perspective;
    }
    void setProjectionOrtho(){
        //projection = false ?//just for testing
       //    glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
       //    glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);
    }
    void customProjection(){
        //projection = false ?//just for testing
       //    glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
       //    glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);
    }
};
class Renderer {
public:
    int x, y;
    GLFWwindow* window;
    ViewMode viewMode;
    set<iDrawCall*> drawcalls;
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

        viewMode.setProjectionPerspective(glm::radians(45.0f), (float)x / (float)y, .1f, 200.0f);
        //projection = false ?//just for testing
        //    glm::ortho(0.f, (float)x, 0.f, (float)y, -.1f, 100.f) :
        //    glm::perspective(glm::radians(45.0f), (float)x/(float)y, .1f, 200.0f);


        glViewport(0, 0, x, y);        
        glEnable(GL_DEPTH_TEST);
        //textureAlpha
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //endTextureAlpha
        
        glfwSwapInterval(1);
    }
    
    void drawRender() {
        for (auto& d : drawcalls)
            d->draw(*this);
    }
    void clearRender() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void swapBuffers() {
        glfwSwapBuffers(window);
        glfwPollEvents();
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
    VertexBuffer vb;
    VertexBufferLayout vl;
    IndexBuffer ib;
    VertexArray va;
public:
    Shader sa;
    glm::mat4 fMat = glm::mat4(1);
    Meshdata() {
        vector<gMesh::MeshVertexData> vertexData;
        vector<unsigned int> index;
        sa.changeShader("default");
        if (gMesh::MeshFunctionsLibrary::LoadMeshVertexData(
            "res/models/suzane.obj",
            vertexData, index)) {
            vl.push<float>(3);
            vl.push<float>(3);
            vl.push<float>(2);
            va.bind();
            vb.setData(vertexData.data(), vertexData.size() * sizeof(gMesh::MeshVertexData));
            ib.setData(index.data(), index.size());
            va.addBuffer(vb, vl);
            va.unbind();
        }
    }
    void draw(const class Renderer& renderer) {
        va.bind();
        sa.bind();
        sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = fMat;
        sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;

        sa.uploadParams();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};

//
//class Rectangle : public iDrawCall{
//    VertexBuffer vb;
//    VertexBufferLayout vl;
//    VertexArray va;
//    IndexBuffer ib;
//    Shader sa;
//    glm::mat4 fMat = glm::mat4(1);
//public:
//    glm::fvec3 pos, rot;
//    glm::fvec4 c = { 1, 1, 1, 1 };
//
//    void color(glm::fvec4 color) {
//        this->c = color;
//    }
//
//    void move(glm::fvec3 pos) {
//        this->pos = pos;
//        fMat = glm::translate(fMat, pos);
//    }
//    void rotate(glm::fvec3 axis, float radians) {
//        fMat = glm::rotate(fMat, radians, axis);
//    }
//
//    Rectangle(){
//        vector<float> vertex;
//        vector<unsigned int> index;
//
//        sa.changeShader("default");
//        vertex = {
//            0, 0,
//            1, 0,
//            1, 1,
//            0, 1,
//        };
//        index = {
//            0, 1, 2,
//            2, 3, 0
//        };
//
//        vl.push<float>(2);
//        va.bind();
//        vb.setData(vertex.data(), vertex.size() * sizeof(float));
//        ib.setData(index.data(), index.size());
//        va.addBuffer(vb, vl);
//        va.unbind();
//    }
//    void draw(const class Renderer& renderer) {
//        va.bind();
//        sa.bind();
//
//        sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = c;
//        sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = fMat;
//        sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;
//        sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mProjection;
//        sa.uploadParams();
//        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
//    }
//};
//class DrawObject : public iDrawCall {
//public:
//    VertexBuffer vb;
//    VertexBufferLayout vl;
//    VertexArray va;
//    IndexBuffer ib;
//    Shader sa;
//    Texture tx;
//
//    vector<float> vertex;
//    vector<unsigned int> index;
//    float a = 0.1;
//    glm::vec3 cubePositions[10] = {
//        glm::vec3(0.0f,  0.0f,  0.0f),
//        glm::vec3(2.0f,  5.0f, -15.0f),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -2.0f, -12.3f),
//        glm::vec3(2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f,  3.0f, -7.5f),
//        glm::vec3(1.3f, -2.0f, -2.5f),
//        glm::vec3(1.5f,  2.0f, -2.5f),
//        glm::vec3(1.5f,  0.2f, -1.5f),
//        glm::vec3(-1.3f,  1.0f, -1.5f)
//    };
//
//    DrawObject() {
//        tx.changeTexture("grid");
//        sa.changeShader("defaultShader");
//        vertex = {
//            0, 0,        0, 0,
//            1, 0,      1, 0,
//            1, 1,    1, 1,
//            0, 1,      0, 1
//        };        
//        index = {
//            0, 1, 2,
//            2, 3, 0
//        };
//        tx.bind();
//        vl.push<float>(2);
//        vl.push<float>(2);
//
//        va.bind();
//        vb.setData(vertex.data(), vertex.size() * sizeof(float));
//        ib.setData(index.data(), index.size());
//        va.addBuffer(vb, vl);
//        va.unbind();
//    }
//    void draw(const class Renderer& renderer) {
//        sa.bind();
//        sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mProjection;
//
//        va.bind();
//        a = a > 50 ? 1 : a + 0.1f;
//        for (int i = 1; i < 10; i++) {
//            auto mPos = glm::mat4(1.0f);
//            
//            mPos = glm::translate(mPos, cubePositions[i]);
//            mPos = glm::rotate(mPos, glm::radians(a *i), glm::vec3(1.f, 1.f, 0.f));
//            sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = mPos;
//
//            sa.uploadParams();
//            glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
//        }
//    }
//};
