
#include "Renderer.h"
#include "Patterns.hpp"


namespace xp {
    
    template<typename T, const int> struct UniformData {};
    template<typename T> struct UniformData<T, 1> { T a; };
    template<typename T> struct UniformData<T, 2> { T a, b; };
    template<typename T> struct UniformData<T, 3> { T a, b, c; };
    template<typename T> struct UniformData<T, 4> { T a, b, c, d; };



    struct ShaderParamBind {
        unsigned int paramLocation;
        unsigned int paramType;
    };
    class ShaderParameter {
    public:
        ShaderParamBind param;
        ShaderParameter(ShaderParameter& other) {
            this->param = other.param;
        }
        void operator=(ShaderParameter const& other) {
            //trick from mr.Joe
        }
        virtual void upload() = 0;
    };



    template<typename T, const int n = 1> class UniformParam {
    public:
        virtual void setParamValue(const unsigned int& paramLocation) = 0;
    };
    
    #define UniExpClass(t, exp)\
    template<> class UniformParam<t> { \
    public:\
        t value;\
        UniformParam(){}\
        UniformParam(t&& init) : value(init) {}\
        void upload(const unsigned int& paramLocation){exp;}};

    #define UniExpPrimitive(t, n, exp) \
    template<> class UniformParam<t, n>{\
        public:\
            UniformData<t, n> value;\
            UniformParam(){}\
            UniformParam(const UniformData<t, n>&& init) : value(init) {}\
            void upload(const unsigned int& paramLocation){exp;}}

    UniExpPrimitive(int, 1, glUniform1i(paramLocation, value.a));
    UniExpPrimitive(int, 2, glUniform2i(paramLocation, value.a, value.b));
    UniExpPrimitive(int, 3, glUniform3i(paramLocation, value.a, value.b, value.c));
    UniExpPrimitive(int, 4, glUniform4i(paramLocation, value.a, value.b, value.c, value.d));
    UniExpPrimitive(float, 1, glUniform1f(paramLocation, value.a));
    UniExpPrimitive(float, 2, glUniform2f(paramLocation, value.a, value.b));
    UniExpPrimitive(float, 3, glUniform3f(paramLocation, value.a, value.b, value.c));
    UniExpPrimitive(float, 4, glUniform4f(paramLocation, value.a, value.b, value.c, value.d));

    UniExpClass(glm::mat4, glUniformMatrix4fv(paramLocation, 1, GL_FALSE, &value[0][0]));
    #undef UniExpPrimitive
    #undef UniExpClass


    

    

    class ParamShaderHelper {
    public:
        static ShaderParameter* reflectType(GLenum type) {
            ShaderParameter* out = 0;
            switch (type) {
            case GL_FLOAT_MAT4:
                break;
            case GL_FLOAT_VEC4:
                break;
            case GL_FLOAT_VEC3:
                break;
            case GL_FLOAT_VEC2:
                break;
            case GL_FLOAT:
                break;
            case GL_INT_VEC4:
                break;
            case GL_INT_VEC3:
                break;
            case GL_INT_VEC2:
                break;
            case GL_INT:
                break;
            case GL_SAMPLER_2D:
                break;
            }
            return out;
        }
    };
    void test() {
        UniformParam<float> s;
        auto p = UniformParam<float, 1>({ .2});
        p.value = { 0.5f };
        cout << endl;
        //ShaderParameter* s = ParamTest<glm::vec1>(glm::vec1(0));
        
    }
};



int main() {
    xp::test();
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