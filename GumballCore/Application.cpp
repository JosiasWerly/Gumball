#include "Engine.hpp"
#include "AssetManager.hpp"

#include "Shaders.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"
#include "Math.hpp"
#include "RenderSystem.hpp"
#include "Event.hpp"

using namespace std;



void foo(int v) {
	cout << "foo" << v << endl;
}
void bar(int v) {
	cout << "bar" << v << endl;
}

int main() {

	EventFnx<int> ea(&foo), eb(&bar);

	Dispatcher<int> dis;
	dis.bind(&ea);
	dis.bind(&eb);

	dis.broadcast(2);



	auto &engine = Engine::instance();
	engine.initialize();

	auto &renderSystem = RenderSystem::instance();
	auto &assetSystem = AssetsSystem::instance();
	assetSystem.loadAllFiles("res\\");
	
	View v;
	v.viewMode.setProjectionOrtho(-1, 1, -1, 1);
	v.transform.position.z -= 5;
	DrawInstance d;
	renderSystem.drawboard.drawInstances.push_back(&d);
	renderSystem.drawboard.views.push_back(&v);
	engine.onPlay();

	while (1) {	
		renderSystem.tick();
		if (InputSystem::isKeyDown(Input::EKeyCode::W))
			v.transform.position.y += 0.01;
		else if (InputSystem::isKeyDown(Input::EKeyCode::S))
			v.transform.position.y -= 0.01;

		if (InputSystem::onKeyReleased(Input::EKeyCode::Q)) {
			v.transform.rotator.rotate(0.f, 5.f, 0.f);
			cout << ">" << endl;
		}
		else if (InputSystem::onKeyPressed(Input::EKeyCode::E)) {
			v.transform.rotator.rotate(0.f, -5.f, 0.f);
			cout << "<" << endl;
		}

		InputSystem::processInputs();
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