
#include "Renderer.h"
#include "Patterns.hpp"

int main() {
    Renderer r;
    r.setup("gumball", 800, 600);
    auto 
        a = r.newDrawCall<debugDraw>(), 
        b = r.newDrawCall<debugDraw>();
    
    auto &sy = ShaderSystem::instance();
    sy.newShaderFromFile("res/shaders/defaultShader.shader");
    sy.newShaderFromFile("res/shaders/red.shader");
    sy.newShaderFromFile("res/shaders/blue.shader");
    
    Shader sA("defaultShader"), sB("defaultShader");
    sA.setParam<UniformParam<float, 4>>("uColor", { 0.f, 1.f, 0.f, 0.f });
    sB.setParam<UniformParam<float, 4>>("uColor", { 1.f, 0.f, 0.f, 0.f });
    auto bfLayout = new VertexBufferLayout;
    bfLayout->push<float>(2);
    a->setup(
        &sA,
        *bfLayout,
        {
            -.1,  -.1,
            .1,  -.1,
            .1, .1,
            -.1,  .1
        },
        { 
            0, 1, 2,
            2, 3, 0 
        }  
    );
    b->setup(
        &sB,
        *bfLayout,
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
    delete a, b;

    glfwTerminate();
    return 0;
}