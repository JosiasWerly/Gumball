

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
    ShaderParameter(){}
    virtual void call() = 0;
};
template<class T, int>class UniformParam : 
    public ShaderParameter{
public:
    void call() {
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
    void call() {
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
    void call() {
        glUniform3f(paramLocation, a, b, c);
    }
};




class Shader {
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

    unsigned int shaderProgram;
    map<string, ShaderParameter*> parameters;
    Shader(string file) {
        shaderProgram = loadShaderFromFile(file);
    }
    ~Shader() {
        glDeleteProgram(shaderProgram);
        for (auto& k : parameters) {
            delete k.second;
        }
        parameters.clear();
    }
    template<class T>void push(string name, T data) {        
        T* param = new T;
        *param = data;
        param->paramLocation = glGetUniformLocation(shaderProgram, name.c_str());
        parameters[name] = param;
    }
    void bind() {
        glDCall(glUseProgram(shaderProgram));
        for (auto &k : parameters){
            k.second->call();
        }
    }
    void unBind() {
        glUseProgram(0);
    }
};
class VertexBuffer{
public:
    unsigned int rendererID;
    VertexBuffer(const void* data, unsigned int size) {
        glDCall(glGenBuffers(1, &rendererID));
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
        glDCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
    ~VertexBuffer() {
        glDCall(glDeleteBuffers(1, &rendererID));
    }
    void bind() const{
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    }
    void unbind() const{
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
class IndexBuffer {
public:
    unsigned int rendererID, count;
    IndexBuffer(unsigned int *indices, unsigned int count) :
        count(count){
        glDCall(glGenBuffers(1, &rendererID));
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
        glDCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
    }
    ~IndexBuffer() {
        glDCall(glDeleteBuffers(1, &rendererID));
    }
    void bind() const {
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
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
    Shader* shader;
    unsigned int vba;



    void setup(Shader* shader, vector<float> data, vector<unsigned int> index) {
        this->shader = shader;
        glGenVertexArrays(1, &vba);
        glBindVertexArray(vba);
        
        vbo = new VertexBuffer(data.data(), data.size() * sizeof(float));
        
        glDCall(glEnableVertexAttribArray(0));
        glDCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));
        ibe = new IndexBuffer(index.data(), index.size());
    }
    void draw() {
        shader->bind();
        //glDCall(glUniform4f(glGetUniformLocation(shader, "uColor"), 1, 0, 0, 1));
        glBindVertexArray(vba);
        glDCall(glDrawElements(GL_TRIANGLES, ibe->getCount(), GL_UNSIGNED_INT, nullptr));
    }
    void terminate() {
        if (vbo) delete vbo;
        if (ibe) delete ibe;
        if (shader) delete shader;
    }
};