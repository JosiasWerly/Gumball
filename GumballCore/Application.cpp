#include "Engine.hpp"
#include "AssetManager.hpp"

#include "Shaders.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Math.hpp"
#include "RenderSystem.hpp"
#include "Event.hpp"

using namespace std;


static void printVector(Vector3 v) {
	cout << "x:" << v.x << " y:" << v.y << " z:" << v.z << endl;
}

int main() {
	auto &engine = Engine::instance();
	engine.initialize();

	auto &renderSystem = RenderSystem::instance();
	auto &assetSystem = AssetsSystem::instance();
	auto &inputSystem = InputSystem::instance();
	assetSystem.loadAllFiles("res\\");
	
	View v;
	v.viewMode.setProjectionPerspective();
	v.transform.position.z = -5;
	renderSystem.pushView(0, &v);

	DrawInstance dd;
	dd.setup();
	renderSystem.pushDrawInstance(0, &dd);


	//DrawInstance *draws[40];
	//float i = 0;
	//for (auto &d : draws) {
	//	d = new DrawInstance;
	//	d->transform.position.z = -5 + rand() % 10;
	//	d->transform.position.x = -5 + rand() % 10;
	//	d->transform.position.y = -5 + rand() % 10;
	//	renderSystem.pushDrawInstance(0, d);
	//	d->setup();
	//	i+=1.0;
	//}
	Vector3 w (1, 0, 0);
	w += w * 0.1;

	engine.onPlay();

	const float vel = 0.01f;
	while (1) {	
		renderSystem.tick();
		inputSystem.tick();

		//i = 0;
		//for (auto& d : draws) {
		//	i+= 0.2;
		//	d->transform.rotator.rotate(0.1, 0, 0);
		//}

		if (inputSystem.isKeyDown(Input::EKeyCode::W)) {
			dd.transform.position += dd.transform.rotator.forward() * vel;
		}
		else if (inputSystem.isKeyDown(Input::EKeyCode::S))
			dd.transform.position -= dd.transform.rotator.forward() * vel;

		if (inputSystem.isKeyDown(Input::EKeyCode::D))
			dd.transform.position += dd.transform.rotator.right() * vel;
		else if (inputSystem.isKeyDown(Input::EKeyCode::A))
			dd.transform.position -= dd.transform.rotator.right() * vel;

		if (inputSystem.isKeyDown(Input::EKeyCode::UP))
			dd.transform.rotator.rotate(1, 0, 0);
		else if (inputSystem.isKeyDown(Input::EKeyCode::DOWN))
			dd.transform.rotator.rotate(-1, 0, 0);

		if (inputSystem.isKeyDown(Input::EKeyCode::LEFT))
			dd.transform.rotator.rotate(0, 0, -1);
		else if (inputSystem.isKeyDown(Input::EKeyCode::RIGHT))
			dd.transform.rotator.rotate(0, 0, 1);

		//printVector(v.transform.position);
		//printVector(dd.transform.rotator.eulerAngles());
		//printVector(dd.transform.rotator.forward());
		//printVector(dd.transform.rotator.right());
		//printVector(dd.transform.rotator.up());
		//cout << "---------------------------" << endl;
	}
	engine.shutdown();
	return 0;
}






/*
* clone assets
* Windown: Viewport, drawcalls
* Gameplay: Actor, Component, GarbageCollector
* UI
*/

// asset factory
// widown
// Gameplay classes: 
// Actor, Component, GarbageCollector
// UI


//!glfwWindowShouldClose(window)




	//glfwInit();
	//
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//
	//GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	//if (window == NULL) {
	//	std::cout << "Failed to create GLFW window" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}
	//glfwMakeContextCurrent(window);
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}
	//cout << glGetString(GL_VERSION) << endl;
	//glfwMakeContextCurrent(window);