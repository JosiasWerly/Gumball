
#include "Renderer.hpp"
#include "Patterns.hpp"



//sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = c;
//sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = fMat;
//sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;
//sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mProjection;


class FpsCounter{
    double lastTime = 0;
    unsigned int frames = 0;
public:
    double ms = 0;
    FpsCounter(){
        lastTime = glfwGetTime();
    }
    void fpsTick() {
        double actualTime = glfwGetTime();
        frames++;
        if (actualTime - lastTime >= 1.0) {
            ms = 1000 / frames;
            frames = 0;
            lastTime += 1.0f;
        }
    }
    double getMsBySec() {
        return ms;
    }
};
FpsCounter fpsCounter;


void processInput(GLFWwindow* window);
int main() {
    Renderer r;
    r.setup("gumball", 800, 600);
    r.viewMode.mProjection = glm::translate(r.viewMode.mProjection, glm::vec3(0, 0, -3));

    auto& sy = ShaderSystem::instance();
    auto& st = TextureSystem::instance();
    auto& sm = MeshSystem::instance();

    sy.loadFromFile("res/shaders/default.shader");
    sy.loadFromFile("res/shaders/unlit.shader");
    sy.loadFromFile("res/shaders/texture.shader");
    
    st.loadFromFile("res/textures/test.png");
    st.loadFromFile("res/textures/gumball.png");
    st.loadFromFile("res/textures/gumbalA.png");
    st.loadFromFile("res/textures/grid.png");

    sm.loadFromFile("res/models/suzane.obj");
    sm.loadFromFile("res/models/torus.obj");

    constexpr int recSize = 2;
    vector<Meshdata*> drawObjects;

    for (size_t i = 0; i < recSize; i++) {
        Meshdata* newMesh = new Meshdata;

        if(drawObjects.size() != 0)
            newMesh->transform.attach(&drawObjects.back()->transform);
        
        newMesh->loadMesh(i % 2 ? "suzane" : "torus");        
        
        newMesh->sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = r.viewMode.mProjection;
        drawObjects.push_back(newMesh);
        r << newMesh;
    }


	//clear all the bind/selectionStack 
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    Meshdata* suz = drawObjects[0];
    Meshdata* tor = drawObjects[1];

    while (!glfwWindowShouldClose(r.window)){
        if (glfwGetKey(r.window, GLFW_KEY_W) == GLFW_PRESS)
            suz->transform.rotation += glm::vec3(0.01, 0, 0);
        else if (glfwGetKey(r.window, GLFW_KEY_S) == GLFW_PRESS)
            suz->transform.rotation += glm::vec3(-0.01, 0, 0);

        if (glfwGetKey(r.window, GLFW_KEY_A) == GLFW_PRESS)
            suz->transform.position += glm::vec3(0, 0, 0.01);
        else if (glfwGetKey(r.window, GLFW_KEY_D) == GLFW_PRESS)
            suz->transform.position += glm::vec3(0, 0, -0.01);

        if (glfwGetKey(r.window, GLFW_KEY_Q) == GLFW_PRESS)
            suz->transform.scale += glm::vec3(0.01, 0, 0);
        else if (glfwGetKey(r.window, GLFW_KEY_E) == GLFW_PRESS)
            suz->transform.scale += glm::vec3(-0.01, 0, 0);


        if (glfwGetKey(r.window, GLFW_KEY_I) == GLFW_PRESS)
            tor->transform.rotation += glm::vec3(0.01, 0, 0);
        else if (glfwGetKey(r.window, GLFW_KEY_K) == GLFW_PRESS)
            tor->transform.rotation += glm::vec3(-0.01, 0, 0);

        if (glfwGetKey(r.window, GLFW_KEY_J) == GLFW_PRESS)
            tor->transform.position += glm::vec3(0, 0, 0.01);
        else if (glfwGetKey(r.window, GLFW_KEY_L) == GLFW_PRESS)
            tor->transform.position += glm::vec3(0, 0, -0.01);

        if (glfwGetKey(r.window, GLFW_KEY_U) == GLFW_PRESS)
            tor->transform.scale += glm::vec3(0.01, 0, 0);
        else if (glfwGetKey(r.window, GLFW_KEY_O) == GLFW_PRESS)
            tor->transform.scale += glm::vec3(-0.01, 0, 0);
        

        r.clearRender();
        r.drawRender();
        r.swapBuffers();
        fpsCounter.fpsTick();
        cout << fpsCounter.getMsBySec() << endl;
    }
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}




//void processInput(GLFWwindow* window);
//int main() {
//
//    Renderer r;
//    r.setup("gumball", 800, 600);
//    r.viewMode.mProjection = glm::translate(r.viewMode.mProjection, glm::vec3(0, 0, -3));
//
//    auto& sy = ShaderSystem::instance();
//    auto& st = TextureSystem::instance();
//    auto& sm = MeshSystem::instance();
//
//    sy.loadFromFile("res/shaders/default.shader");
//    sy.loadFromFile("res/shaders/unlit.shader");
//    sy.loadFromFile("res/shaders/texture.shader");
//
//    st.loadFromFile("res/textures/test.png");
//    st.loadFromFile("res/textures/gumball.png");
//    st.loadFromFile("res/textures/gumbalA.png");
//    st.loadFromFile("res/textures/grid.png");
//
//    sm.loadFromFile("res/models/suzane.obj");
//
//    constexpr int recSize = 1;
//    list<Meshdata*> rectangles;
//    for (size_t i = 0; i < recSize; i++)
//        rectangles.push_back(new Meshdata);
//
//
//    for (auto& rec : rectangles) {
//        //rec->fMat = glm::translate(rec->fMat, glm::vec3(rand() % 50 - 25, rand() % 50 - 25, rand() % 50 - 25));
//        rec->sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = r.viewMode.mProjection;
//        r << rec;
//    }
//
//
//    //clear all the bind/selectionStack 
//    glUseProgram(0);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//    Transform t;
//    float cLenght = 2.f;
//    float a = .3f;
//    while (!glfwWindowShouldClose(r.window)) {
//        if (glfwGetKey(r.window, GLFW_KEY_W) == GLFW_PRESS) {
//            a += 0.1f;
//            r.viewMode.mView = glm::lookAt(
//                glm::vec3(sin(a) * cLenght, 0.0, cos(a) * cLenght),
//                glm::vec3(0.0, 0.0, 0.0),
//                glm::vec3(0.0, 1.0, 0.0)
//            );
//
//        }
//        else if (glfwGetKey(r.window, GLFW_KEY_S) == GLFW_PRESS) {
//            a -= 0.1f;
//            r.viewMode.mView = glm::lookAt(
//                glm::vec3(sin(a) * cLenght, 0.0, cos(a) * cLenght),
//                glm::vec3(0.0, 0.0, 0.0),
//                glm::vec3(0.0, 1.0, 0.0)
//            );
//        }
//
//
//        r.clearRender();
//        r.drawRender();
//        r.swapBuffers();
//        fpsCounter.fpsTick();
//        cout << fpsCounter.getMsBySec() << endl;
//    }
//    sy.unloadAll();
//    r.drawcalls.clear();
//    glfwTerminate();
//    return 0;
//}