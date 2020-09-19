#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <iostream>
#include <set>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

#define glAssert(fnx) if (!(fnx)) __debugbreak();
#define glDCall(fnx)\
GLClearError();\
fnx;\
glAssert(GLLogCall(#fnx, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* fnx, const char* file, int line) {
    while (GLenum err = glGetError()) {
        cout << "[openGL "<< err << "] "<< fnx << " " << file << " " << line;
        return false;
    }
    return true;
}






class iDrawCall {
public:
    virtual void draw() = 0;
};
class Renderer {
public:
    int x, y;
    GLFWwindow* window;
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
        glViewport(0, 0, x, y);
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
};

#include <fstream>
#include <sstream>

class ShaderParameter{
public:
    unsigned int paramLocation;
    ShaderParameter(){
    }
    virtual void upload() = 0;

};
template<class T, int>class UniformParam : 
    public ShaderParameter{
public:
    void upload() {
        __debugbreak();
    }
};
template<class T>class UniformParam<T,4> :
    public ShaderParameter {
public:
    T a, b, c, d;
    UniformParam(){}
    UniformParam(T a, T b, T c, T d) :
        a(a), b(b), c(c), d(d){
    }
    void upload() {
        glUniform4f(paramLocation, a, b, c, d);
    }
};
template<class T>class UniformParam<T,3> :
    public ShaderParameter {
public:
    T a, b, c;
    UniformParam(){}
    UniformParam(T a, T b, T c) :
        a(a), b(b), c(c){
    }
    void upload() {
        glUniform3f(paramLocation, a, b, c);
    }
};



class ShaderHelper{
    ShaderHelper(const ShaderHelper&) = delete;
public:
    struct ShaderSource {
        string vertex, fragment;
    };
    static ShaderSource loadShaderCode(string filePath) {
        ifstream fileStream(filePath);
        string line;
        bool a = false;
        ShaderSource out;
        string outString[2];
        enum eShaderType {
            none = -1, vertex, fragment
        } eShType;
        while (getline(fileStream, line)) {
            if (line.find("#shader vertex") != string::npos)
                eShType = eShaderType::vertex;
            else if (line.find("#shader fragment") != string::npos)
                eShType = eShaderType::fragment;
            else if (line.find("//") == string::npos)
                outString[eShType] += line + "\n";
        }
        return { outString[eShaderType::vertex], outString[eShaderType::fragment] };
    }
    static int compileShader(unsigned int ShaderType, const string& code) {
        unsigned int id = glCreateShader(ShaderType);
        const char* src = code.c_str();
        glShaderSource(id, 1, &src, 0);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* msg = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, msg);

            cout << (ShaderType == GL_VERTEX_SHADER ? "vert" : "frag") << " shader failed to compile =(" << endl;
            return 0;
        }
        return id;
    }
    static int buildShader(const string& vertexShader, const string& fragmentShader) {
        unsigned int
            p = glCreateProgram(),
            vs = compileShader(GL_VERTEX_SHADER, vertexShader),
            fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(p, vs);
        glAttachShader(p, fs);
        glLinkProgram(p);
        glValidateProgram(p); //???

        int valid;
        glGetProgramiv(p, GL_LINK_STATUS, &valid);
        if (!valid)
            cout << "program shader error" << endl;
        glDeleteShader(vs);
        glDeleteShader(fs);
        return p;
    }
    static int loadShaderFromFile(string filePath) {
        auto shaderCode = loadShaderCode(filePath);
        return buildShader(shaderCode.vertex, shaderCode.fragment);
    }
    static map<string, ShaderParameter*> getActiveParameters(const unsigned int shaderProgram) {
        map<string, ShaderParameter*> out;
        int typeCount;
        const unsigned int bufSize = 16; //?
        unsigned int type;
        int length, size, paramLocation;
        char name[bufSize];
        //GL_ACTIVE_UNIFORMS
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &typeCount);
        for (int i = 0; i < typeCount; i++) {
            glGetActiveAttrib(shaderProgram, (GLuint)i, bufSize, &length, &size, &type, name);
            paramLocation = glGetUniformLocation(shaderProgram, name);
            ShaderParameter* shParam = nullptr;
            switch (type) {
            case GL_FLOAT_VEC4:
                shParam = new UniformParam<float, 4>(1, 1, 1, 1);
                shParam->paramLocation = paramLocation;
                break;
            case GL_FLOAT_VEC3:
                shParam = new UniformParam<float, 3>(1, 1, 1);
                shParam->paramLocation = paramLocation;
                break;
            }
            if (shParam)
                out[name] = shParam;
        }
        return out;
    }
};
class Shader {
public:
    unsigned int shaderProgram;
    map<string, ShaderParameter*> parameters;
    void clearParameters() {
        for (auto& k : parameters)
            delete k.second;
        parameters.clear();
    }
public:
    Shader() {
    }
    ~Shader() {
        if (shaderProgram)
            glDeleteProgram(shaderProgram);
        clearParameters();
    }
    void compile(string file){
        shaderProgram = ShaderHelper::loadShaderFromFile(file);
        clearParameters();
        parameters = ShaderHelper::getActiveParameters(shaderProgram);
    }
    
    template<class T>bool setParam(string name, T data) {
        auto it = parameters.find(name);
        if (it != parameters.end()) {
            *it->second = data;
            return true;
        }
        return false;
    }
    template<class T>const T* getParam(string name) {
        auto it = parameters.find(name);
        if (it != parameters.end())
            return dynamic_cast<T*>(it->second);
        return nullptr;
    }
    void bind() {
        glDCall(glUseProgram(shaderProgram));
        for (auto &k : parameters){
            k.second->upload();
        }
    }
    void unBind() {
        glUseProgram(0);
    }


    
};
class VertexBuffer{
public:
    unsigned int bufferID;
    VertexBuffer(const void* data, unsigned int size) {
        glDCall(glGenBuffers(1, &bufferID));
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
        glDCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    ~VertexBuffer() {
        glDCall(glDeleteBuffers(1, &bufferID));
    }
    void bind() const{
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
    }
    void unbind() const{
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

struct VertexBufferElement {
    unsigned int type, count;
    unsigned char normalized;

    static unsigned int getSizeType(const unsigned int type) {
        switch (type) {
        case GL_FLOAT:  return 4;
        case GL_INT:    return 4;
        case GL_BYTE:   return 1;
        }
        __debugbreak();
        return -1;
    }
};
class VertexBufferLayout {
public:
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
        stride += VertexBufferElement::getSizeType(GL_FLOAT) * count;
    }    
    template<>void push<int>(unsigned int count) {
        elements.push_back({ GL_INT, count, false });
        stride += VertexBufferElement::getSizeType(GL_INT) * count;
    }
    template<>void push<char>(unsigned int count) {
        elements.push_back({ GL_BYTE, count, false });
        stride += VertexBufferElement::getSizeType(GL_BYTE) * count;
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
    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout& layout) {
        bind();
        vb.bind();
        const auto& elements = layout.elements;
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++){
            const auto& element = elements[i];
            glDCall(glEnableVertexAttribArray(i));
            glDCall(glVertexAttribPointer(i, element.count, element.type, 
                element.normalized, layout.getStride(), (const void*)offset));
            offset += element.count;
        }
    }
};
class IndexBuffer {
public:
    unsigned int bufferID, count;
    IndexBuffer(unsigned int *indices, unsigned int count) :
        count(count){
        glDCall(glGenBuffers(1, &bufferID));
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
        glDCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    }
    ~IndexBuffer() {
        glDCall(glDeleteBuffers(1, &bufferID));
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

class debugDraw : public iDrawCall {
public:
    VertexBuffer* vbo;
    IndexBuffer* ibe;
    VertexArray* vba;
    Shader* shader;



    void setup(Shader* shader, VertexBufferLayout layout, vector<float> data, vector<unsigned int> index) {
        this->shader = shader;
        vba = new VertexArray;
        vba->bind();
        vbo = new VertexBuffer(data.data(), data.size() * sizeof(float));
        vba->addBuffer(*vbo, layout);
        
        //glDCall(glEnableVertexAttribArray(0));
        //glVertexAttribPointer()
        //glDCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));
        ibe = new IndexBuffer(index.data(), index.size());
    }
    void draw() {
        shader->bind();
        vba->bind();
        glDCall(glDrawElements(GL_TRIANGLES, ibe->getCount(), GL_UNSIGNED_INT, nullptr));
    }
    void terminate() {
        if (vbo) delete vbo;
        if (ibe) delete ibe;
        if (shader) delete shader;
    }
};