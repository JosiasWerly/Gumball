
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
    st.loadTexture("res/textures/grid.png");

	auto DrawCallA = r.newDrawCall<debugDraw>();
	auto bfLayoutA = new VertexBufferLayout;
	Texture tA("test");
	Shader sA("defaultShader");
    tA.bind();
	bfLayoutA->push<float>(2);
	bfLayoutA->push<float>(2);
	sA.setParam<UniformParam<float, 4>>("uColor", { 0.f, 1.f, 0.f, 0.f });
	sA.setParam <UniformParam<int, 1>>("uTexture", 0);
	DrawCallA->setup(
		&sA,
		*bfLayoutA,
		{
			-.1, -.1,   0, 0,
			 .1, -.1,   1, 0,
			 .1, .1,    1, 1,
			-.1, .1,    0, 1
		},
		{
			0, 1, 2,
			2, 3, 0
		}
    );
	DrawCallA->text = &tA;
    

    auto DrawCallB = r.newDrawCall<debugDraw>();
    auto bfLayoutB = new VertexBufferLayout;
    Shader sB("red");
    
    sB.setParam<UniformParam<float,4>>("uColor", { 1.f, 0.f, 0.f, 0.f });
    bfLayoutB->push<float>(2);
    DrawCallB->setup(
        &sB,
        *bfLayoutB,
        {
            -.4,  -.2,
            -.4,  -.0,
            -.2, .0,
            -.2,  -.2
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