#include "Engine.hpp"
#include "AssetManager.hpp"

#include "Shaders.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Math.hpp"
#include "RenderSystem.hpp"
#include "Event.hpp"

using namespace std;




int main() {
	auto &engine = Engine::instance();
	engine.initialize();

	auto &renderSystem = RenderSystem::instance();
	auto &assetSystem = AssetsSystem::instance();
	auto &inputSystem = InputSystem::instance();
	assetSystem.loadAllFiles("res\\");
	
	View v;
	v.viewMode.setProjectionPerspective();
	v.transform.position.z -= 5;
	DrawInstance d;

	renderSystem.drawboard.drawInstances.push_back(&d);
	renderSystem.drawboard.views.push_back(&v);
	engine.onPlay();

	while (1) {	
		renderSystem.tick();
		inputSystem.tick();



		if (inputSystem.isKeyDown(Input::EKeyCode::W))
			v.transform.position += v.transform.rotator.forward() * 0.01f;
		else if (inputSystem.isKeyDown(Input::EKeyCode::S))
			v.transform.position += v.transform.rotator.forward() * -0.01f;

		if (inputSystem.isKeyDown(Input::EKeyCode::A))
			v.transform.position += v.transform.rotator.right() * 0.01f;
		else if (inputSystem.isKeyDown(Input::EKeyCode::D))
			v.transform.position += v.transform.rotator.right() * -0.01f;

		if (inputSystem.isKeyDown(Input::EKeyCode::Q))
			v.transform.rotator.rotate(0, 0, 1);
		else if (inputSystem.isKeyDown(Input::EKeyCode::E))
			v.transform.rotator.rotate(0, 0, -1);
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