
#include "Renderer.h"
#include "Patterns.hpp"


namespace xp {
    struct ShaderBind {
        string name;
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
    

    class Shader {
        typedef map<string, ShaderParameter*> Uniforms;
    protected:
        const ShaderBind shaderBind;
    public:
        Uniforms uniforms;
        Shader(ShaderBind shaderRef) : shaderBind(shaderRef) {}
    };
    class ParamShaderHelper {
    public:
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
};

int main() {
    Renderer r;
    r.setup("gumball", 800, 600);

    auto& sy = ShaderSystem::instance();
    auto& st = TextureSystem::instance();

    sy.loadFromFile("res/shaders/defaultShader.shader");
    sy.loadFromFile("res/shaders/red.shader");
    sy.loadFromFile("res/shaders/blue.shader");    
    st.loadFromFile("res/textures/test.png");
    st.loadFromFile("res/textures/gumball.png");
    st.loadFromFile("res/textures/gumbalA.png");
    st.loadFromFile("res/textures/grid.png");
    

	auto DrawCallA = r.newDrawCall<debugDraw>();
	Texture tA("grid");
	Shader sA("defaultShader");
    tA.bind();
	sA.setParam<UniformParam<float, 4>>("uColor", { 1.f, 1.f, 1.f, 0.5f });
    sA.setParam<UniformParam<int, 1>>("uTexture", {0});
    sA.setParam<UniformParam<glm::mat4, 1>>("mvp", r.projection);
    DrawCallA->text = &tA;
    
    auto bfLayoutA = new VertexBufferLayout;
	bfLayoutA->push<float>(2);
    bfLayoutA->push<float>(2);
	DrawCallA->setup(
		&sA,
		*bfLayoutA,
		{
			0, 0,        0, 0,
			100, 0,      1, 0,
            100, 100,    1, 1,
			0, 100,      0, 1
		},
		{
			0, 1, 2,
			2, 3, 0
		}
    );	
    /*{
        0, 0, 0, 0,
            0.5, 0, 1, 0,
            0.5, 0.5, 1, 1,
            0, 0.5, 0, 1
    }*/

    //auto DrawCallB = r.newDrawCall<debugDraw>();
    //Shader sB("defaultShader");
    //Texture tB("gumball");
    //DrawCallB->text = &tB;
    //tB.bind();    
    //sB.setParam<UniformParam<float, 4>>("uColor", { 1.f, 1.f, 1.f, 1.f });
    //sB.setParam<UniformParam<int, 1>>("uTexture", { 0 });
    //
    //auto bfLayoutB = new VertexBufferLayout;
    //bfLayoutB->push<float>(2);
    //bfLayoutB->push<float>(2);
    //DrawCallB->setup(
    //    &sB,
    //    *bfLayoutB,
    //    {
    //        -.4,  -.2,  0, 0,
    //        -.2,  -.2,  1, 0,
    //        -.2,   .0,  1, 1,
    //        -.4,  -.0,  0, 1
    //    },
    //    {
    //        0, 1, 2,
    //        2, 3, 0
    //    }
    //);

    //clear all the bind/selectionStack
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    while (!glfwWindowShouldClose(r.window)){
        r.clear();       
        r.draw();
        r.swap();
    }
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}