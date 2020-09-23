#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "Patterns.hpp"
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

namespace gbLib {
    string getNameOfFilePath(string path) {
        string fName = std::filesystem::path(path).filename().string();
        return fName.substr(0, fName.find_last_of("."));
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

    template<typename T>T* newDrawCall() {
        T* out = new T;
        drawcalls.insert((iDrawCall*)out);
        return out;
    }
    void delDrawCall(iDrawCall* drawCall) {
        auto i = drawcalls.find(drawCall);
        if (i != drawcalls.end()) {
            delete (*i);
            drawcalls.erase(i);
        }
    }
};


//ShaderSystem
#include <fstream>
#include <sstream>
struct ShaderParameterValues{
    unsigned int paramLocation;
    unsigned int paramType;
};
class ShaderParameter {
public:
    unsigned int paramLocation;
    unsigned int paramType;
    ShaderParameter() {
    }
    ShaderParameter(ShaderParameter& other) {
        this->paramLocation = other.paramLocation;
        this->paramType = other.paramType;
    }
    void operator=(ShaderParameter const& other){
        //trick from mr.Joe
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
template<class T>class UniformParam<T,2> :
    public ShaderParameter {
public:
    T a, b;
    UniformParam() {}
    UniformParam(T a, T b) :
        a(a), b(b){
    }
    void upload() {
        glUniform2f(paramLocation, a, b);
    }
};
template<class T>class UniformParam<T,1> :
    public ShaderParameter {
public:
    T a;
    UniformParam() {}
    UniformParam(T a) :
        a(a){
    }
    void upload() {
        glUniform1f(paramLocation, a);
    }
};

template<>class UniformParam<int, 4> :
    public ShaderParameter {
public:
    int a, b, c, d;
    UniformParam() {}
    UniformParam(int a, int b, int c, int d) :
        a(a), b(b), c(c), d(d) {
    }
    void upload() {
        glUniform4i(paramLocation, a, b, c, d);
    }
};
template<>class UniformParam<int, 3> :
    public ShaderParameter {
public:
    int a, b, c;
    UniformParam() {}
    UniformParam(int a, int b, int c) :
        a(a), b(b), c(c) {
    }
    void upload() {
        glUniform3i(paramLocation, a, b, c);
    }
};
template<>class UniformParam<int, 2> :
    public ShaderParameter {
public:
    int a, b;
    UniformParam() {}
    UniformParam(int a, int b) :
        a(a), b(b) {
    }
    void upload() {
        glUniform2i(paramLocation, a, b);
    }
};
template<>class UniformParam<int, 1> :
    public ShaderParameter {
public:
    int a;
    UniformParam() {}
    UniformParam(int a) :
        a(a) {
    }
    void upload() {
        glUniform1i(paramLocation, a);
    }
};
template<>class UniformParam<glm::mat4, 1 > :
    public ShaderParameter {
public:
    glm::mat4 a;
    UniformParam() {}
    UniformParam(glm::mat4 a) :
        a(a) {
    }
    void upload() {
        glUniformMatrix4fv(paramLocation, 1, GL_FALSE, &a[0][0]);
    }
};





class ShaderHelper {
    ShaderHelper(const ShaderHelper&) = delete;
public:
    struct ShaderSource {
        string vertex, fragment;
    };
    static ShaderSource loadShaderCodeFromFile(string filePath) {
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
        glDCall(glShaderSource(id, 1, &src, 0));
        glDCall(glCompileShader(id));

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* msg = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, msg);

            cout << (ShaderType == GL_VERTEX_SHADER ? "vert" : "frag") << msg << endl;
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
        glValidateProgram(p);

        int valid;
        glGetProgramiv(p, GL_LINK_STATUS, &valid);
        if (!valid)
            cout << "program shader error" << endl;
        glDeleteShader(vs);
        glDeleteShader(fs);
        return p;
    }
    static int buildShaderFile(string filePath) {
        auto shaderCode = loadShaderCodeFromFile(filePath);
        return buildShader(shaderCode.vertex, shaderCode.fragment);
    }
    
    static ShaderParameter* reflectUniformParam(unsigned int paramLocation, GLenum type) {
        ShaderParameter* shParam = nullptr;
        switch (type) {
        case GL_FLOAT_MAT4:
            shParam = new UniformParam<glm::mat4, 1>(glm::mat4());
            shParam->paramLocation = paramLocation;
            break;
        case GL_FLOAT_VEC4:
            shParam = new UniformParam<float, 4>(1, 1, 1, 1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_FLOAT_VEC3:
            shParam = new UniformParam<float, 3>(1, 1, 1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_FLOAT_VEC2:
            shParam = new UniformParam<float, 2>(1, 1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_FLOAT:
            shParam = new UniformParam<float, 1>(1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_INT_VEC4:
            shParam = new UniformParam<int, 4>(1, 1, 1, 1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_INT_VEC3:
            shParam = new UniformParam<int, 3>(1, 1, 1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_INT_VEC2:
            shParam = new UniformParam<int, 2>(1, 1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_INT:
            shParam = new UniformParam<int, 1>(1);
            shParam->paramLocation = paramLocation;
            break;
        case GL_SAMPLER_2D:
            shParam = new UniformParam<int, 1>(1);
            shParam->paramLocation = paramLocation;
            break;
        }
        return shParam;
    }
    static map<string, ShaderParameter*> getActiveUniforms(const unsigned int shaderProgram) {
        map<string, ShaderParameter*> out;
        int typeCount;
        const unsigned int bufSize = 16; //?
        unsigned int type;
        int length, size, paramLocation;
        char name[bufSize];
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &typeCount);//GL_ACTIVE_UNIFORMS
        
        for (int i = 0; i < typeCount; i++) {
            glGetActiveUniform(shaderProgram, (GLuint)i, bufSize, &length, &size, &type, name);
            paramLocation = glGetUniformLocation(shaderProgram, name);
            ShaderParameter* shParam = reflectUniformParam(paramLocation, type);
            if (shParam)
                out[name] = shParam;
        }
        return out;
    }
};
struct ShaderReference {
    const string name;
    const unsigned int programId;
};

class ShaderSystem : 
    public Singleton<ShaderSystem> {
private:
    AssetSystem<ShaderReference> loadedShaders;
public:
    void loadShaderFromFile(string filePath) {
        /*string fName = std::filesystem::path(filePath).filename().string();
        fName = fName.substr(0, fName.find_last_of("."));*/

        string fName = gbLib::getNameOfFilePath(filePath);
        auto shaderCode = ShaderHelper::loadShaderCodeFromFile(filePath);
        loadShader(fName, shaderCode.vertex, shaderCode.fragment);
    }
    void loadShader(string name, string vertex, string fragment) {
        if (!loadedShaders.contain(name)) {
            unsigned int shaderProgram = ShaderHelper::buildShader(vertex, fragment);
            loadedShaders.push(name, { name, shaderProgram });
        }
    }
    void unloadShader(string name) {
        AssetSystem<ShaderReference>::it i;
        loadedShaders.contain(name, i);
        glDeleteProgram(i->second->programId);
        loadedShaders.pop(name);
    }
    void clearAllShaders() {
        auto assets = loadedShaders.getAssets();
        for (auto k : assets)
            glDeleteProgram(k.second->programId);
        loadedShaders.clear();
    }
    ShaderReference const& getShaderReference(string name) {
        return *loadedShaders.get(name);
    }
};
class Shader {
    typedef map<string, ShaderParameter*> Uniforms;
protected:
    const ShaderReference& shaderRef;
public:
    Uniforms uniforms;
    Shader(string name) :
        shaderRef(ShaderSystem::instance().getShaderReference(name)){
        updateParams();
    }
    void updateParams() {
        uniforms = ShaderHelper::getActiveUniforms(shaderRef.programId);
    }
    template<class T>void setParam(string name, T data) {
        auto it = uniforms.find(name);
        if (it != uniforms.end())
            *((T*)it->second) = data;
    }
    template<class T>const T* getParam(string name) {
        auto it = uniforms.find(name);
        if (it != uniforms.end())
            return dynamic_cast<T*>(it->second);
        return nullptr;
    }
    void bind() {
        glDCall(glUseProgram(shaderRef.programId));
        for (auto& k : uniforms)
            k.second->upload();
    }
    void unBind() {
        glUseProgram(0);
    }
};

#include "stb_image.h"
struct TextureReference{
    const unsigned int glBufferId;
    const unsigned char* memoryBuffer;
    const int width, height;
};
class TextureSystem : 
    public Singleton<TextureSystem> {
    AssetSystem<TextureReference> loadedTextures;
public:
    void loadTexture(string filePath) {
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
            loadedTextures.push(gbLib::getNameOfFilePath(filePath), { bufferId, imageBuffer, x, y });
        }
        else throw;
    }
    void unloadTexture(string name) {
        auto s = loadedTextures.get(name);
        glDeleteTextures(1, &s->glBufferId);
        delete s->memoryBuffer;
        loadedTextures.pop(name);
    }
    const TextureReference& getTextureReference(string name) {
        return *loadedTextures.get(name);
    }
};
class Texture {
public:
    const TextureReference& textureData;
    Texture(string name) :
        textureData(TextureSystem::instance().getTextureReference(name)) {
    }
    void bind(char slot = 0) {
        glDCall(glActiveTexture(GL_TEXTURE0 + slot));
        glDCall(glBindTexture(GL_TEXTURE_2D, textureData.glBufferId));
    }
    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
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
        elements.push_back({ GL_FLOAT, count,false });
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
            offset += element.count*element.getSizeType(element.type);
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
    VertexArray* vba;
    VertexBuffer* vbo;
    IndexBuffer* ibe;
    Shader* shader;
    Texture *text;

    void setup(Shader* shader, VertexBufferLayout layout, vector<float> data, vector<unsigned int> index) {
        this->shader = shader;
        vba = new VertexArray;
        vba->bind();
        vbo = new VertexBuffer(data.data(), data.size() * sizeof(float));
        vba->addBuffer(*vbo, layout);        
        ibe = new IndexBuffer(index.data(), index.size());
        
    }
    void draw() {
        if (text)
            text->bind();
        shader->bind();

        vba->bind();
        glDCall(glDrawElements(GL_TRIANGLES, ibe->getCount(), GL_UNSIGNED_INT, nullptr));
        if (text)
            text->unbind();
    }
    void terminate() {
        if (vbo) delete vbo;
        if (ibe) delete ibe;
        if (shader) delete shader;
    }
};