
#include "Renderer.hpp"
#include "Patterns.hpp"


//sa.getParam("uColor")->value<Uniform<glm::fvec4>>().data = c;
//sa.getParam("uModel")->value<Uniform<glm::mat4>>().data = fMat;
//sa.getParam("uView")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mView;
//sa.getParam("uProj")->value<Uniform<glm::mat4>>().data = renderer.viewMode.mProjection;






void processInput(GLFWwindow* window);
int main() {
    Gumball::Window w;
    w.create("Gumball", 800, 600);
    double t = w.getAspec();
        

    //r.viewMode.mView = glm::translate(r.viewMode.mView, glm::vec3(0, 0, -3));

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

    
    Renderer r(&w);
    Camera* c = r.camera = new Camera();
    c->transform.position += glm::vec3(0, 0, -200);
    
    constexpr int recSize = 100;
    vector<Meshdata*> drawObjects;

    for (size_t i = 0; i < recSize; i++) {
        Meshdata* newMesh = new Meshdata;

        //if(drawObjects.size() != 0)
        //    newMesh->transform.attach(&drawObjects.back()->transform);
        
        if (i % 2)
            newMesh->loadMesh("suzane" );
        else 
            newMesh->loadMesh("torus");
        newMesh->transform.position += glm::vec3(rand() % 100 -50, rand() % 100-50, rand() % 100-50);
        drawObjects.push_back(newMesh);
        r << newMesh;
    }



    Meshdata* suz = drawObjects[0];
    Meshdata* tor = drawObjects[1];

    w.clearGLStack();
    while (!glfwWindowShouldClose(r.window)){
        if (glfwGetKey(r.window, GLFW_KEY_W) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, 0, 0.1);
        else if (glfwGetKey(r.window, GLFW_KEY_S) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, 0, -0.1);

        if (glfwGetKey(r.window, GLFW_KEY_A) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, 0.1, 0);
		else if (glfwGetKey(r.window, GLFW_KEY_D) == GLFW_PRESS)
            c->transform.rotation += glm::vec3(0, -0.1, 0);
		       
        w.clearBuffer();
        r.drawRender();
        w.swapBuffers();
        cout << w.getMS() << endl;
    }
    sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}

/*
##### Gumball 0.1 #####
-create asset system
-resolve drawcall, camera and uniforms system

-collision detection
-traces, lines
*/