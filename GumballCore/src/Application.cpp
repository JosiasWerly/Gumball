
#include "Renderer.hpp"
#include "Patterns.hpp"





//vClip =mProj*mView*mMode*mLocal
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
    
    
    DrawObject *d = new DrawObject;
    d->sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };

    d->sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = r.projection;
    d->sa.getParam("uView")->value<Uniform<glm::mat4>>().data = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -3.0f));
    d->sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = glm::mat4(1.0f);
    r << d;

	//clear all the bind/selectionStack 
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    while (!glfwWindowShouldClose(r.window)){
        r.clearRender();       
        r.drawRender();
        r.swapBuffers();
    }
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}
