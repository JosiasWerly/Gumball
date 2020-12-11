#include "Gumball.hpp"

void processInput(GLFWwindow* window);
int main() {
	window.create("Gumball", 800, 600);
	assetManager.loadAssets("res/");

	Camera* mainCamera = render.camera = new Camera();
	mainCamera->transform.position += glm::vec3(0, 0, -10);

	constexpr int recSize = 2;
	float pi = 3.14159265359;
	vector<Meshdata*> drawObjects;
	for (size_t i = 0; i < recSize; i++) {
		Meshdata* newMesh = new Meshdata;
		if (i % 2) {
			newMesh->loadMesh("plane");
			newMesh->transform.position += glm::vec3(0, 2, 0);
			newMesh->transform.rotation.rotate(90, 0, 0);
		}
		else {
			newMesh->loadMesh("cube");
			newMesh->transform.position += glm::vec3(0, -2, 0);
		}
		drawObjects.push_back(newMesh);
		render << newMesh;
	}
	float turnRate = 4;


	Transform* t = &mainCamera->transform;
	while (true) {
		if (t) {
			if (engine.keyPressed(GLFW_KEY_W))
				t->rotation.rotate(turnRate, 0, 0);
			else if (engine.keyPressed(GLFW_KEY_S))
				t->rotation.rotate(-turnRate, 0, 0);

			if (engine.keyPressed(GLFW_KEY_Q))
				t->rotation.rotate(0, turnRate, 0);
			else if (engine.keyPressed(GLFW_KEY_E))
				t->rotation.rotate(0, -turnRate, 0);

			if (engine.keyPressed(GLFW_KEY_A))
				t->rotation.rotate(0, 0, turnRate);
			else if (engine.keyPressed(GLFW_KEY_D))
				t->rotation.rotate(0, 0, -turnRate);
		}
		window.disposeRender();
	}
	return 0;
}

/*
##### Gumball 0.1 #####
     refator:

[ok] resolve drawcall
[ok] camera

[mk1] uniforms system
[DOING] texture && UV
[TODO] input wrapper
[todo] separe gamethread vs drawThread && querrySystem
[todo] collision detection
[todo] traces, lines
*/


