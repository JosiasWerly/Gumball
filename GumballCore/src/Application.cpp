#include "Gumball.hpp"


int main() {
	window.create("Gumball", 800, 600);
	assetManager.loadAssets("res/");
	
	Camera* mainCamera = render.currentContext->camera = new Camera();
	mainCamera->transform.position += glm::vec3(0, 0, -100);
	MeshComponent* toTest;
	constexpr int recSize = 2;
	for (size_t i = 0; i < recSize; i++) {
		Actor* ac = new Actor;
		MeshComponent *meshComp = toTest = new MeshComponent();
		*ac << meshComp;
		meshComp->mesh.changeMesh("torus");
		if (i % 2) {
			ac->transform.position += glm::vec3(0, 2, 0);
			ac->transform.rotation.rotate(90, 0, 0);
		}
		else {
			ac->transform.position += glm::vec3(0, -2, 0);
		}
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

			if (engine.keyPressed(GLFW_KEY_0) && toTest) {
				(*toTest->getOwner()) >> toTest;
				delete toTest;
			}
		}
		memory.tick();
		window.clearBuffer();
		render.disposeRender();
		window.swapBuffers();
	}
	return 0;
}









//int main() {
//	window.create("Gumball", 800, 600);
//	assetManager.loadAssets("res/");
//
//	Camera* mainCamera = render.camera = new Camera();
//	mainCamera->transform.position += glm::vec3(0, 0, -10);
//
//	constexpr int recSize = 2;
//	vector<MeshDrawable*> drawObjects;
//	for (size_t i = 0; i < recSize; i++) {
//		MeshDrawable* newMesh = new MeshDrawable;
//		newMesh->transform = new Transform;
//		if (i % 2) {
//			newMesh->changeMesh("cube");
//			newMesh->transform->position += glm::vec3(0, 2, 0);
//			newMesh->transform->rotation.rotate(90, 0, 0);
//		}
//		else {
//			newMesh->transform->position += glm::vec3(0, -2, 0);
//		}
//		drawObjects.push_back(newMesh);
//		render << newMesh;
//	}
//	float turnRate = 4;
//
//
//	Transform* t = &mainCamera->transform;
//	while (true) {
//		if (t) {
//			if (engine.keyPressed(GLFW_KEY_W))
//				t->rotation.rotate(turnRate, 0, 0);
//			else if (engine.keyPressed(GLFW_KEY_S))
//				t->rotation.rotate(-turnRate, 0, 0);
//
//			if (engine.keyPressed(GLFW_KEY_Q))
//				t->rotation.rotate(0, turnRate, 0);
//			else if (engine.keyPressed(GLFW_KEY_E))
//				t->rotation.rotate(0, -turnRate, 0);
//
//			if (engine.keyPressed(GLFW_KEY_A))
//				t->rotation.rotate(0, 0, turnRate);
//			else if (engine.keyPressed(GLFW_KEY_D))
//				t->rotation.rotate(0, 0, -turnRate);
//		}
//		window.disposeRender();
//		memory.tick();
//	}
//	return 0;
//}








/*
##### Gumball 0.1 #####
     refator:
[mk1] uniforms system
[mk1] texture && UV
[TODO] input wrapper
[todo] separe gamethread vs drawThread && querrySystem
[todo] collision detection
[todo] traces, lines
*/