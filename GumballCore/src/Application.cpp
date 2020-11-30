#include "Renderer.hpp"
#include "Patterns.hpp"

void processInput(GLFWwindow* window);
int main() {
    Window w;
    w.create("Gumball", 800, 600);

    auto& am = AssetManager::instance();    
    am.pushFactory("shader", new ShaderFactory);
    am.pushFactory("mesh", new MeshFactory);
    am.pushFactory("texture", new TextureFactory);

    am.loadAssetFromDisk("res/shaders/default.shader");
    am.loadAssetFromDisk("res/shaders/unlit.shader");
    am.loadAssetFromDisk("res/shaders/texture.shader");

    am.loadAssetFromDisk("res/textures/test.png");
    am.loadAssetFromDisk("res/textures/gumball.png");
    am.loadAssetFromDisk("res/textures/gumbalA.png");
    am.loadAssetFromDisk("res/textures/grid.png");
        
    am.loadAssetFromDisk("res/models/torus.obj");
    am.loadAssetFromDisk("res/models/suzane.obj");


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
    //sy.unloadAll();
    r.drawcalls.clear();
    glfwTerminate();
    return 0;
}

/*
##### Gumball 0.1 #####
     refator:

[ok] uniforms system
[mk1] resolve drawcall, 
[mk1] camera
[mk1] create asset system
[todo] collision detection
[todo] traces, lines
*/