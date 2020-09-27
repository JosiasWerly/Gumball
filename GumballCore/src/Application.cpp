
#include "Renderer.h"
#include "Patterns.hpp"


namespace xp {
    //class Shader {
    //    typedef map<string, ShaderParameter*> Uniforms;
    //protected:
    //    const ShaderBind shaderBind;
    //public:
    //    Uniforms uniforms;
    //    Shader(ShaderBind shaderRef) : shaderBind(shaderRef) {}
    //};
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
	//sA.setParam<Uniform<float, 4>>("uColor", );
    //sA.setParam<Uniform<int, 1>>("uTexture", {0});
    //sA.setParam<Uniform<glm::mat4, 1>>("mvp", r.projection);
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