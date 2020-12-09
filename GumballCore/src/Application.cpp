#include "Gumball.hpp"

void processInput(GLFWwindow* window);
int main() {
    FVector 
        a(1, 1, 1),
        b(2, 2, 2);

    window.create("Gumball", 800, 600);
    assetManager.loadAssetFromDisk("res/shaders/default.shader");
    assetManager.loadAssetFromDisk("res/shaders/unlit.shader");
    assetManager.loadAssetFromDisk("res/shaders/texture.shader");

    assetManager.loadAssetFromDisk("res/textures/test.png");
    assetManager.loadAssetFromDisk("res/textures/gumball.png");
    assetManager.loadAssetFromDisk("res/textures/gumbalA.png");
    assetManager.loadAssetFromDisk("res/textures/grid.png");
    
    assetManager.loadAssetFromDisk("res/models/torus.obj");
    assetManager.loadAssetFromDisk("res/models/cube.obj");
    assetManager.loadAssetFromDisk("res/models/plane.obj");
    assetManager.loadAssetFromDisk("res/models/sphere.obj");
    //assetManager.loadAssetFromDisk("res/models/suzane.obj");
    Camera* mainCamera = render.camera = new Camera();
    mainCamera->transform.position += glm::vec3(0, 0, -10);

    constexpr int recSize = 2;
    vector<Meshdata*> drawObjects;
    for (size_t i = 0; i < recSize; i++) {
        Meshdata* newMesh = new Meshdata;
        if (i % 2) {
            newMesh->loadMesh("sphere");
            newMesh->transform.position += glm::vec3(0, 2, 0);
        }
        else {
            newMesh->loadMesh("cube");
            newMesh->transform.position += glm::vec3(0, -2, 0);
        }
        //newMesh->transform.position += glm::vec3(rand() % 50 - 25, rand() % 50- 25, rand() % 50 - 25);
        drawObjects.push_back(newMesh);
        render << newMesh;
    }
    float turnRate = 0.01;
    while (true){
        Transform* t = &render.camera->transform;
        cout << t->rotation.right().rawVec().x << " " << t->rotation.right().rawVec().y << " " << t->rotation.right().rawVec().z << endl;
        if (engine.keyPressed(GLFW_KEY_W))
            t->rotation.rotate(-turnRate, 0, 0);
        else if (engine.keyPressed(GLFW_KEY_S))
            t->rotation.rotate(turnRate, 0, 0);
        
        if (engine.keyPressed(GLFW_KEY_Q))
            t->rotation.rotate(0, -turnRate, 0);
        else if (engine.keyPressed(GLFW_KEY_E))
            t->rotation.rotate(0, turnRate, 0);

        if (engine.keyPressed(GLFW_KEY_A))
            t->rotation.rotate(0, 0, -turnRate);
        else if (engine.keyPressed(GLFW_KEY_D))
            t->rotation.rotate(0, 0, turnRate);
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