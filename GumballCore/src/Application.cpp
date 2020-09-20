
#include "Renderer.h"
int main() {
    Renderer r;
    r.setup("gumball", 800, 600);
    auto a = new debugDraw, b = new debugDraw;
    r.drawcalls.insert(a);
    r.drawcalls.insert(b);
    
    auto &sy = ShaderSystem::getInstance();
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


//int main() {
//    Window w;
//    w.setup("gumball", 800, 600);
//    auto d = new debugDraw;
//    w.drawcalls.insert(d);
//
//    float vertices[] = {
//        -.5,  -.5,
//        .5,  -.5,
//        .5, .5,
//        -.5,  .5
//    };
//    unsigned int vIndex[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//    auto shaderCode = loadShaderCode("res/shaders/defaultShader.shader");
//    unsigned int shader = createShader(shaderCode.vertex, shaderCode.fragment);
//
//
//    //bus of data
//    unsigned int va = 0;
//    glGenVertexArrays(1, &va);
//    glBindVertexArray(va);
//
//
//    // bus of data to send
//    unsigned int vb = 0;
//    glGenBuffers(1, &vb);
//    glBindBuffer(GL_ARRAY_BUFFER, vb);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//size of the data to send
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);//bind this vbo to vao
//
//    unsigned int ve = 0;
//    glGenBuffers(1, &ve);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ve);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndex), vIndex, GL_STATIC_DRAW); //??
//
//
//    //clear all the bind/selectionStack
//    glUseProgram(0);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    //
//    while (true) {
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//
//        glUseProgram(shader);
//        glUniform4f(glGetUniformLocation(shader, "uColor"), .0, 0.1, 0.2, 1);
//        glBindVertexArray(va);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ve);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//        //endDrawCall
//
//        w.clear();
//
//    }
//
//    glDeleteVertexArrays(1, &va);
//    glDeleteBuffers(1, &ve);
//    glDeleteBuffers(1, &vb);
//    glDeleteProgram(shader);
//    return 0;
//}