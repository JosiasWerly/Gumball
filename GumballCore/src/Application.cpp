
#include "Renderer.hpp"
#include "Patterns.hpp"


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
    tA.bind();
	Shader sA("defaultShader");
    sA.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };
    sA.getParam("mvp")->value<Uniform<glm::mat4>>().data = r.projection;
    
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