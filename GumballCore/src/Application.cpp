
#include "Renderer.h"
#include "Patterns.hpp"

int main() {
    Renderer r;
    r.setup("gumball", 800, 600);

    auto& sy = ShaderSystem::instance();
    auto& st = TextureSystem::instance();

    sy.loadShaderFromFile("res/shaders/defaultShader.shader");
    sy.loadShaderFromFile("res/shaders/red.shader");
    sy.loadShaderFromFile("res/shaders/blue.shader");    
    st.loadTexture("res/textures/test.png");
    st.loadTexture("res/textures/gumball.png");
    st.loadTexture("res/textures/gumbalA.png");
    st.loadTexture("res/textures/grid.png");
    

	auto DrawCallA = r.newDrawCall<debugDraw>();
	Texture tA("gumbalA");
	Shader sA("defaultShader");
    tA.bind();
	sA.setParam<UniformParam<float, 4>>("uColor", { 1.f, 1.f, 1.f, 0.5f });
    sA.setParam<UniformParam<int, 1>>("uTexture", {0});
    DrawCallA->text = &tA;
	
    
    auto bfLayoutA = new VertexBufferLayout;
	bfLayoutA->push<float>(2);
    bfLayoutA->push<float>(2);
	DrawCallA->setup(
		&sA,
		*bfLayoutA,
		{
			0, 0,      0, 0,
			.9, 0,     1, 0,
			.9, .9,    1, 1,
			.0, .9,    0, 1
		},
		{
			0, 1, 2,
			2, 3, 0
		}
    );
	
    

    auto DrawCallB = r.newDrawCall<debugDraw>();
    Shader sB("defaultShader");
    Texture tB("gumball");
    DrawCallB->text = &tB;
    tB.bind();    
    sB.setParam<UniformParam<float, 4>>("uColor", { 1.f, 1.f, 1.f, 1.f });
    sB.setParam<UniformParam<int, 1>>("uTexture", { 0 });
    
    auto bfLayoutB = new VertexBufferLayout;
    bfLayoutB->push<float>(2);
    bfLayoutB->push<float>(2);
    DrawCallB->setup(
        &sB,
        *bfLayoutB,
        {
            -.4,  -.2,  0, 0,
            -.2,  -.2,  1, 0,
            -.2,   .0,  1, 1,
            -.4,  -.0,  0, 1
        },
        {
            0, 1, 2,
            2, 3, 0
        }
    );

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
    sy.clearAllShaders();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}