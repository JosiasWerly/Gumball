#include "Gumball.hpp"

void processInput(GLFWwindow* window);
int main() {
    window.create("Gumball", 800, 600);
    assetManager.loadAssetFromDisk("res/shaders/default.shader");
    assetManager.loadAssetFromDisk("res/shaders/unlit.shader");
    assetManager.loadAssetFromDisk("res/shaders/texture.shader");

    assetManager.loadAssetFromDisk("res/textures/test.png");
    assetManager.loadAssetFromDisk("res/textures/gumball.png");
    assetManager.loadAssetFromDisk("res/textures/gumbalA.png");
    assetManager.loadAssetFromDisk("res/textures/grid.png");
    
    assetManager.loadAssetFromDisk("res/models/torus.obj");
    assetManager.loadAssetFromDisk("res/models/suzane.obj");


    
    Camera* c = render.camera = new Camera();
    c->transform.position += glm::vec3(0, 0, -200);
    
    constexpr int recSize = 100;
    vector<Meshdata*> drawObjects;

    for (size_t i = 0; i < recSize; i++) {
        Meshdata* newMesh = new Meshdata;
        if (i % 2)
            newMesh->loadMesh("suzane" );
        else 
            newMesh->loadMesh("torus");
        newMesh->transform.position += glm::vec3(rand() % 100 -50, rand() % 100-50, rand() % 100-50);
        drawObjects.push_back(newMesh);
        render << newMesh;
    }

    while (!window.shouldClose()){
        if (engine.keyPressed(GLFW_KEY_W))
            c->transform.rotation += glm::vec3(0, 0, 0.1);
        else if (engine.keyPressed(GLFW_KEY_S))
            c->transform.rotation += glm::vec3(0, 0, -0.1);

        if (engine.keyPressed(GLFW_KEY_A))
            c->transform.rotation += glm::vec3(0, 0.1, 0);
		else if (engine.keyPressed(GLFW_KEY_D))
            c->transform.rotation += glm::vec3(0, -0.1, 0);

        window.disposeRender();
    }
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