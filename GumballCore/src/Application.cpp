
#include "Renderer.hpp"
#include "Patterns.hpp"





void processInput(GLFWwindow* window);
int main() {
    Renderer r;
    r.setup("gumball", 800, 600);
    r.viewMode.mProjection = glm::translate(r.viewMode.mProjection, glm::vec3(0, 0, -10));

    auto& sy = ShaderSystem::instance();
    auto& st = TextureSystem::instance();

    sy.loadFromFile("res/shaders/defaultShader.shader");
    sy.loadFromFile("res/shaders/red.shader");
    sy.loadFromFile("res/shaders/blue.shader");
    st.loadFromFile("res/textures/test.png");
    st.loadFromFile("res/textures/gumball.png");
    st.loadFromFile("res/textures/gumbalA.png");
    st.loadFromFile("res/textures/grid.png");
    
    
    //DrawObject *d = new DrawObject;
    //
    //d->sa.bind();
    //d->sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = { 1, 1, 1, 1 };
    ////d->sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = r.projection;
    //d->sa.getParam("uView")->value<Uniform<glm::mat4>>().data = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -3.0f));
    //d->sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = glm::mat4(1.0f);
    //r << d;


    Rectangle rec[10];
    glm::vec3 cubePositions[10] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    float a = .3f;
    for(int i=0; i<10; i++){
        r << &rec[i];
        rec[i].move(cubePositions[i]);
        rec[i].rotate({ 0, 0, 1 }, glm::radians(a * i));
        rec[i].color({ i/10.f, i / 10.f, i / 10.f, 1 });
    }


	//clear all the bind/selectionStack 
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);



    while (!glfwWindowShouldClose(r.window)){
        for (int i = 0; i < 10; i++) {
            rec[i].rotate({ 1, 1, 1 }, glm::radians(0.1 * i));
        }

        if (glfwGetKey(r.window, GLFW_KEY_W) == GLFW_PRESS) {
            a += 0.1f;
            r.viewMode.mView = glm::lookAt(
                glm::vec3(sin(a) * 10.0f, 0.0, cos(a) * 10.0f),
                glm::vec3(0.0, 0.0, 0.0),
                glm::vec3(0.0, 1.0, 0.0)
            );
            //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            //r.viewMode.mView = glm::translate(r.viewMode.mView, glm::vec3{ 0.1 });
            
        }
        if (glfwGetKey(r.window, GLFW_KEY_S) == GLFW_PRESS) {
            a -= 0.1f;
            r.viewMode.mView = glm::lookAt(
                glm::vec3(sin(a) * 10.0f, 0.0, cos(a) * 10.0f),
                glm::vec3(0.0, 0.0, 0.0),
                glm::vec3(0.0, 1.0, 0.0)
            );
            //r.viewMode.mView = glm::translate(r.viewMode.mView, glm::vec3{ -0.1 });
        }




        r.clearRender();       
        r.drawRender();
        r.swapBuffers();
    }
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}