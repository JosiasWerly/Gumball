#pragma once
#ifndef _shader
#define _shader

#include "Gumball.hpp"
#include "Patterns.hpp"
#include <fstream>
#include <sstream>


struct ShaderParamBind {
    unsigned int paramLocation;
    unsigned int paramType;
};
class iParamStorage {
public:
    virtual void pushParam(const ShaderParamBind& param) = 0;
    virtual void* getPtr() = 0;
};
class ShaderParam {
public:
    ShaderParamBind paramBind;
    iParamStorage* paramStorage = nullptr;

    void setStorage(unsigned int type);
    void bind() {
        paramStorage->pushParam(paramBind);
    }
};
#pragma region dont_look_at_this
//general case
template<typename t> class Uniform : public iParamStorage {
    virtual void pushParam(const ShaderParamBind& param) = 0;
    virtual void* getPtr() = 0;
};

//specific case
#define UniformDefaultExp(t, exp)\
template<> class Uniform<t> : public iParamStorage {\
    public:\
    t data = t();\
    void operator=(t data) { this->data = data; }\
    void pushParam(const ShaderParamBind& param) { exp; }\
    void* getPtr(){return &data;}}

UniformDefaultExp(int, glUniform1iv(param.paramLocation, 1, &data));
UniformDefaultExp(float, glUniform1fv(param.paramLocation, 1, &data));
UniformDefaultExp(glm::mat4, glUniformMatrix4fv(param.paramLocation, 1, GL_FALSE, &data[0][0]));
UniformDefaultExp(glm::fvec2, glUniform2fv(param.paramLocation, 1, glm::value_ptr(data)));
UniformDefaultExp(glm::fvec3, glUniform3fv(param.paramLocation, 1, glm::value_ptr(data)));
UniformDefaultExp(glm::fvec4, glUniform4fv(param.paramLocation, 1, glm::value_ptr(data)));
UniformDefaultExp(glm::ivec2, glUniform2iv(param.paramLocation, 1, glm::value_ptr(data)));
UniformDefaultExp(glm::ivec3, glUniform3iv(param.paramLocation, 1, glm::value_ptr(data)));
UniformDefaultExp(glm::ivec4, glUniform4iv(param.paramLocation, 1, glm::value_ptr(data)));
#undef UniformDefaultExp
#pragma endregion

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

    static map<string, ShaderParam*> getActiveUniforms(const unsigned int shaderProgram) {
        map<string, ShaderParam*> out;
        int typeCount;
        const unsigned int bufSize = 16; //?
        unsigned int type;
        int length, size, paramLocation;
        char name[bufSize];
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &typeCount);
        for (int i = 0; i < typeCount; i++) {
            glGetActiveUniform(shaderProgram, (GLuint)i, bufSize, &length, &size, &type, name);
            paramLocation = glGetUniformLocation(shaderProgram, name);
            ShaderParam* shParam = new ShaderParam;
            shParam->paramBind = { (unsigned int)paramLocation, type };
            shParam->setStorage(type);
            out[name] = shParam;
        }
        return out;
    }
    static iParamStorage* reflectGLEnum(GLenum type) {
        iParamStorage* out = 0;
        switch (type) {
        case GL_FLOAT:          out = new Uniform<float>;       break;
        case GL_INT:            out = new Uniform<int>;         break;
        case GL_SAMPLER_2D:     out = new Uniform<int>;         break;


        case GL_FLOAT_VEC2:     out = new Uniform<glm::fvec2>;      break;
        case GL_FLOAT_VEC3:     out = new Uniform<glm::fvec3>;      break;
        case GL_FLOAT_VEC4:     out = new Uniform<glm::fvec4>;      break;
        case GL_INT_VEC4:       out = new Uniform<glm::ivec4>;      break;
        case GL_INT_VEC3:       out = new Uniform<glm::ivec3>;      break;
        case GL_INT_VEC2:       out = new Uniform<glm::ivec2>;      break;
        case GL_FLOAT_MAT4:     out = new Uniform<glm::mat4>;       break;
        default:
            cout << "reflectGLEnumError" << endl;
            throw;
        }
        return out;
    }
};
void ShaderParam::setStorage(unsigned int type) {
    if (paramStorage)
        delete paramStorage;
    paramStorage = ShaderFunctionsLibrary::reflectGLEnum(type);
}

struct ShaderBind {
    string name;
    unsigned int programId;
};


class ShaderFactory :
    public iAssetFactory{
public:
    bool canBuild(const string& filePath) {
        return gbLib::getExtOfFilePath(filePath) == "shader";
    }
    iAsset* loadFromDisk(const string& filePath) {
        string fName = gbLib::getNameOfFilePath(filePath);
        auto shaderCode = ShaderFunctionsLibrary::loadShaderCodeFromFile(filePath);
        loadShader(fName, shaderCode.vertex, shaderCode.fragment);
        return nullptr;
    }
    iAsset* unLoad(void* data) {
        //gMap<ShaderBind>::it i;
        //assets.contain(name, i);
        //glDeleteProgram(i->second->programId);
        //assets.pop(name);
        return nullptr;
    }
    void loadShader(string name, string vertex, string fragment) {        
        //    unsigned int shaderProgram = ShaderFunctionsLibrary::buildShader(vertex, fragment);
        //    assets.push(name, { name, shaderProgram });
        //}
    }
};

class ShaderParameters {
protected:
    ShaderBind& shaderbindRef;
public:
    typedef map<string, ShaderParam*> Uniforms;
    Uniforms uniforms;

    ShaderParameters(ShaderBind& shaderbindRef) :
        shaderbindRef(shaderbindRef){
    }
    void retrieveParams() {
        uniforms = ShaderFunctionsLibrary::getActiveUniforms(shaderbindRef.programId);
    }
    void uploadParams() {
        for (auto& k : uniforms)
            k.second->bind();
    }
    template<class T> T& get(string key) {
        static char returnFail[64];
        auto it = uniforms.find(key);
        if (it != uniforms.end())
            return *reinterpret_cast<T*>((it)->second->paramStorage->getPtr());
        return *(T*)&returnFail;
    }
};
class Shader {
protected:
    ShaderBind shaderBind;
public:
    ShaderParameters params;
    
    Shader() : 
        params(shaderBind){
    }
    Shader(string name) : 
        params(shaderBind) {
        changeShader(name);
    }
    
    void changeShader(string name) {
        //TODO
        //shaderBind = ShaderSystem::instance().getAsset(name);
        //params.retrieveParams();
    }
    void bind() {
        glDCall(glUseProgram(shaderBind.programId));
    }
    void unBind() {
        glUseProgram(0);
    }
};
#endif // !_shader
