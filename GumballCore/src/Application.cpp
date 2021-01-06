#include "Gumball.hpp"
#include "lang.hpp"

struct Fun : 
	public iProject {
	
	Viewport* mainCamera = new Viewport;
	MeshComponent* toTest;
	float turnRate = 4;
	Transform* t = &mainCamera->transform;
	virtual void setup() {
		mainCamera->transform.position += glm::vec3(0, 0, -20);
		constexpr int recSize = 2;
		for (size_t i = 0; i < recSize; i++) {
			Actor* ac = new Actor;
			MeshComponent* meshComp = toTest = new MeshComponent();
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
	}
	virtual void tick() {
		if (t) {
			if (Input::isKeyDown(eKeyboard::W))
				t->rotation.rotate(turnRate, 0, 0);
			else if (Input::isKeyDown(eKeyboard::S))
				t->rotation.rotate(-turnRate, 0, 0);

			if (Input::onPressed(eKeyboard::Q)) {
				t->rotation.rotate(0, 45, 0);
				cout << "<<" << endl;
			}
			else if (Input::onRelease(eKeyboard::Q)) {
				t->rotation.rotate(0, -45, 0);
				cout << ">>" << endl;
			}

			if (Input::isKeyDown(eKeyboard::A))
				t->rotation.rotate(0, 0, turnRate);
			else if (Input::isKeyDown(eKeyboard::B))
				t->rotation.rotate(0, 0, -turnRate);

			if (Input::isKeyDown(eKeyboard::UP) && toTest) {
				(*toTest->getOwner()) >> toTest;
				delete toTest;
				toTest = nullptr;
			}
		}
	}
};

int main() {
	Engine& engine = Engine::instance();
	engine.setup();
	engine.window->config("Gameplay test", 800, 600);
	engine.assetManager->loadAssets("res/");
	
	engine.proj = new Fun;
	engine.tick();
	return 0;
}









/*
##### Gumball 0.1 #####
refator:
[mk3] render and window relation
[mk2] asset system, that can support runtime load
[mk1] project sample
[mk1] separe gamethread vs drawThread && querrySystem

[todo] collision detection
[todo] traces, lines
*/






//int main() {
//	engine.setup();
//	engine.window->config("Gameplay test", 800, 600);
//	engine.assetManager->loadAssets("res/");
//
//	///GAMEPLAY	
//	Viewport* mainCamera = new Viewport;
//	mainCamera->transform.position += glm::vec3(0, 0, -20);
//	MeshComponent* toTest;
//	constexpr int recSize = 2;
//	for (size_t i = 0; i < recSize; i++) {
//		Actor* ac = new Actor;
//		MeshComponent* meshComp = toTest = new MeshComponent();
//		*ac << meshComp;
//		meshComp->mesh.changeMesh("torus");
//		if (i % 2) {
//			ac->transform.position += glm::vec3(0, 2, 0);
//			ac->transform.rotation.rotate(90, 0, 0);
//		}
//		else {
//			ac->transform.position += glm::vec3(0, -2, 0);
//		}
//	}
//	float turnRate = 4;
//
//
//	Transform* t = &mainCamera->transform;
//
//
//	while (true) {
//		if (t) {
//			if (Input::isKeyDown(eKeyboard::W))
//				t->rotation.rotate(turnRate, 0, 0);
//			else if (Input::isKeyDown(eKeyboard::S))
//				t->rotation.rotate(-turnRate, 0, 0);
//
//			if (Input::onPressed(eKeyboard::Q)) {
//				t->rotation.rotate(0, 45, 0);
//				cout << "<<" << endl;
//			}
//			else if (Input::onRelease(eKeyboard::Q)) {
//				t->rotation.rotate(0, -45, 0);
//				cout << ">>" << endl;
//			}
//
//			if (Input::isKeyDown(eKeyboard::A))
//				t->rotation.rotate(0, 0, turnRate);
//			else if (Input::isKeyDown(eKeyboard::B))
//				t->rotation.rotate(0, 0, -turnRate);
//
//			if (Input::isKeyDown(eKeyboard::UP) && toTest) {
//				(*toTest->getOwner()) >> toTest;
//				delete toTest;
//				toTest = nullptr;
//			}
//		}
//		engine.tick(); //this has to go away
//	}
//	///end GAMEPLAY
//	return 0;
//}
