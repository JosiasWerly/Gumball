#include "Gumball.hpp"
#include "Shader.hpp"



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


class iDrawCall {
public:
    virtual void draw() = 0;
};
class Renderer {
public:
    int x, y;
    GLFWwindow* window;
    glm::mat4 projection;
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

        float 
            xRatio = 1.f/x,
            yRatio = 1.f/y;

        projection = glm::ortho(0.f, (float)x, 0.f, (float)y, -1.f, 1.f);
        glViewport(0, 0, x, y);
        
        //textureAlpha
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //endTextureAlpha
        
        glfwSwapInterval(1);
    }
    void draw() {
        for (auto& d : drawcalls)
            d->draw();
    }
    void clear() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void swap() {
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

class VertexBuffer {
public:
    unsigned int bufferID;
    VertexBuffer() {
        glDCall(glGenBuffers(1, &bufferID));
    }
    ~VertexBuffer() {
        glDCall(glDeleteBuffers(1, &bufferID));
    }
    void setData(const void* data, unsigned int size) {
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
        glDCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    void bind() const {
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
    }
    void unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
class VertexBufferLayout {
public:
    struct VertexBufferElement {
        unsigned int type, count;
        unsigned char normalized;
    };

    static unsigned int getSizeType(const unsigned int type) {
        switch (type) {
        case GL_FLOAT:  return 4;
        case GL_INT:    return 4;
        case GL_BYTE:   return 1;
        }
        __debugbreak();
        return -1;
    }

    int stride;
    vector<VertexBufferElement> elements;

    int getStride() const {
        return stride;
    }
    const vector<VertexBufferElement>& getElements() {
        return elements;
    }
    template<typename T>void push(unsigned int) {}
    template<>void push<float>(unsigned int count) {
        elements.push_back({ GL_FLOAT, count, false });
        stride += VertexBufferLayout::getSizeType(GL_FLOAT) * count;
    }
    template<>void push<int>(unsigned int count) {
        elements.push_back({ GL_INT, count, false });
        stride += VertexBufferLayout::getSizeType(GL_INT) * count;
    }
    template<>void push<char>(unsigned int count) {
        elements.push_back({ GL_BYTE, count, false });
        stride += VertexBufferLayout::getSizeType(GL_BYTE) * count;
    }
};
class VertexArray {
public:
    unsigned int bufferId;
    VertexArray() {
        glGenVertexArrays(1, &bufferId);
    }
    ~VertexArray() {
        glDeleteVertexArrays(1, &bufferId);
    }
    void bind() {
        glBindVertexArray(bufferId);
    }
    void unbind() {
        glBindVertexArray(0);
    }
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
        bind();
        vb.bind();
        const auto& elements = layout.elements;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++) {
            const auto& element = elements[i];
            glDCall(glEnableVertexAttribArray(i));
            glDCall(glVertexAttribPointer(i, element.count, element.type,
                element.normalized, layout.getStride(), (const void*)offset));
            offset += element.count * VertexBufferLayout::getSizeType(element.type);
        }
    }
};
class IndexBuffer {
public:
    unsigned int bufferID, count;
    IndexBuffer() {
        glDCall(glGenBuffers(1, &bufferID));
    }
    ~IndexBuffer() {
        glDCall(glDeleteBuffers(1, &bufferID));
    }
    void setData(unsigned int* indices, unsigned int count) {
        this->count = count;
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
        glDCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    }
    void bind() const {
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
    }
    void unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    unsigned int getCount() const {
        return count;
    }
};

class DrawObject : public iDrawCall {
public:
    VertexBuffer vb;
    VertexBufferLayout vl;
    VertexArray va;
    IndexBuffer ib;
    Shader sa;
    Texture tx;

    vector<float> vertex;
    vector<unsigned int> index;

    DrawObject() {
        tx.changeTexture("grid");
        sa.changeShader("defaultShader");
        vertex = {
            0, 0,        0, 0,
            100, 0,      1, 0,
            100, 100,    1, 1,
            0, 100,      0, 1
        };
        index = {
            0, 1, 2,
            2, 3, 0
        };

        tx.bind();
        vl.push<float>(2);
        vl.push<float>(2);

        va.bind();
        vb.setData(vertex.data(), vertex.size() * sizeof(float));
        ib.setData(index.data(), index.size());
        va.addBuffer(vb, vl);
        va.unbind();
    }
    void draw() {
        tx.bind();
        sa.bind();
        va.bind();
        glDCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }
};
