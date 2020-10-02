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


struct ShaderBind {
    const string name;
    const unsigned int programId;
};
struct ShaderParamBind {
    unsigned int paramLocation;
    unsigned int paramType;
};
class iShaderParam {
public:
    ShaderParamBind param;
    virtual void push() = 0;
};


template<typename T, const int> struct UniformData {};
template<typename T> struct UniformData<T, 1> { T a; };
template<typename T> struct UniformData<T, 2> { T a, b; };
template<typename T> struct UniformData<T, 3> { T a, b, c; };
template<typename T> struct UniformData<T, 4> { T a, b, c, d; };

template<typename t, const int n = 0> class Uniform : public iShaderParam {
public:
    UniformData<t, n> data;
    Uniform(UniformData<t, n>&& init) : data(init) {}
    void operator=(UniformData<t, n>&& init) {
        data = init;
    }
    void push() {
    }
};
#define paramExp(t, n, exp)\
    template<> class Uniform<t, n> : public iShaderParam{\
    public:\
        UniformData<t, n> data;\
        Uniform(UniformData<t, n>&& init) : data(init){}\
        void push() {exp;}}
paramExp(float, 1, glUniform1f(param.paramLocation, data.a));
paramExp(float, 2, glUniform2f(param.paramLocation, data.a, data.b));
paramExp(float, 3, glUniform3f(param.paramLocation, data.a, data.b, data.c));
paramExp(float, 4, glUniform4f(param.paramLocation, data.a, data.b, data.c, data.d));
paramExp(int, 1, glUniform1i(param.paramLocation, data.a));
paramExp(int, 2, glUniform2i(param.paramLocation, data.a, data.b));
paramExp(int, 3, glUniform3i(param.paramLocation, data.a, data.b, data.c));
paramExp(int, 4, glUniform4i(param.paramLocation, data.a, data.b, data.c, data.d));

template<> class Uniform<glm::mat4, 0> : public iShaderParam {
public:
    glm::mat4 data;
    Uniform(glm::mat4&& init) :
        data(init) {
    }
    void push() {
        glUniformMatrix4fv(param.paramLocation, 1, GL_FALSE, &data[0][0]);
    }
};

class ShaderFunctionsLibrary {
    ShaderFunctionsLibrary(const ShaderFunctionsLibrary&) = delete;
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
    
    static iShaderParam* reflectGLEnum(GLenum type) {
        iShaderParam* out = 0;
        switch (type) {
        case GL_FLOAT_MAT4:
            out = new Uniform<glm::mat4>(glm::mat4());
            break;
        case GL_FLOAT_VEC4:
            out = new Uniform<float, 4>({ 0, 0, 0, 0 });
            break;
        case GL_FLOAT_VEC3:
            out = new Uniform<float, 3>({ 0, 0, 0 });
            break;
        case GL_FLOAT_VEC2:
            out = new Uniform<float, 2>({ 0, 0 });
            break;
        case GL_FLOAT:
            out = new Uniform<float, 1>({ 0 });
            break;
        case GL_INT_VEC4:
            out = new Uniform<int, 4>({ 0, 0, 0, 0 });
            break;
        case GL_INT_VEC3:
            out = new Uniform<int, 3>({ 0, 0, 0 });
            break;
        case GL_INT_VEC2:
            out = new Uniform<int, 2>({ 0, 0 });
            break;
        case GL_INT:
            out = new Uniform<int, 1>({ 0 });
            break;
        case GL_SAMPLER_2D:
            out = new Uniform<int, 1>({ 0 });
            break;
        }
        return out;
    }
    static map<string, iShaderParam*> getActiveUniforms(const unsigned int shaderProgram) {
        map<string, iShaderParam*> out;
        int typeCount;
        const unsigned int bufSize = 16; //?
        unsigned int type;
        int length, size, paramLocation;
        char name[bufSize];
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &typeCount);
        for (int i = 0; i < typeCount; i++) {
            glGetActiveUniform(shaderProgram, (GLuint)i, bufSize, &length, &size, &type, name);
            paramLocation = glGetUniformLocation(shaderProgram, name);
            iShaderParam* shParam = reflectGLEnum(type);
            if (shParam) {
                shParam->param.paramLocation = paramLocation;
                shParam->param.paramType = type;
                out[name] = shParam;
            }
        }
        return out;
    }
};
class ShaderSystem : 
    public AssetFactory<ShaderBind>,
    public Singleton<ShaderSystem> {
public:
    void loadFromFile(string filePath) {
        /*string fName = std::filesystem::path(filePath).filename().string();
        fName = fName.substr(0, fName.find_last_of("."));*/

        string fName = gbLib::getNameOfFilePath(filePath);
        auto shaderCode = ShaderFunctionsLibrary::loadShaderCodeFromFile(filePath);
        loadShader(fName, shaderCode.vertex, shaderCode.fragment);
    }
    void unload(string name) {
        AssetCollection<ShaderBind>::it i;
        assets.contain(name, i);
        glDeleteProgram(i->second->programId);
        assets.pop(name);
    }
    void unloadAll() {        
        auto assetsRef = assets.getAssets();
        for (auto k : assetsRef)
            glDeleteProgram(k.second->programId);
        assets.clear();
    }
    void loadShader(string name, string vertex, string fragment) {
        if (!assets.contain(name)) {
            unsigned int shaderProgram = ShaderFunctionsLibrary::buildShader(vertex, fragment);
            assets.push(name, { name, shaderProgram });
        }
    }
};
class Shader {
	typedef map<string, iShaderParam*> Uniforms;
protected:
	const ShaderBind shaderBind;
public:
	Uniforms uniforms;
	Shader(string name) :
        shaderBind(ShaderSystem::instance().getAsset(name)) {
		updateParams();
	}
	Shader(ShaderBind shaderRef) : 
        shaderBind(shaderRef) {
    }
    void updateParams() {
		uniforms = ShaderFunctionsLibrary::getActiveUniforms(shaderBind.programId);
	}

    template<class T>T* getParam(string name) {
        auto it = uniforms.find(name);
        if (it != uniforms.end())
            return dynamic_cast<T*>((it)->second);
    }

	void bind() {
		glDCall(glUseProgram(shaderBind.programId));
		for (auto& k : uniforms)
			k.second->push();
	}
	void unBind() {
		glUseProgram(0);
	}
};

//class Shader {
//    typedef map<string, ShaderParameter*> Uniforms;
//protected:
//    const ShaderReference& shaderRef;
//public:
//    Uniforms uniforms;
//    Shader(string name) :
//        shaderRef(ShaderSystem::instance().getAsset(name)){
//        updateParams();
//    }
//    void updateParams() {
//        uniforms = ShaderFunctionsLibrary::getActiveUniforms(shaderRef.programId);
//    }
//    template<class T>void setParam(string name, T data) {
//        auto it = uniforms.find(name);
//        if (it != uniforms.end())
//            *((T*)it->second) = data;
//    }
//    template<class T>const T* getParam(string name) {
//        auto it = uniforms.find(name);
//        if (it != uniforms.end())
//            return dynamic_cast<T*>(it->second);
//        return nullptr;
//    }
//    void bind() {
//        glDCall(glUseProgram(shaderRef.programId));
//        for (auto& k : uniforms)
//            k.second->upload();
//    }
//    void unBind() {
//        glUseProgram(0);
//    }
//};

#include "stb_image.h"
struct TextureReference{
    const unsigned int glBufferId;
    const unsigned char* memoryBuffer;
    const int width, height;
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
    const TextureReference& textureData;
    Texture(string name) :
        textureData(TextureSystem::instance().getAsset(name)) {
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