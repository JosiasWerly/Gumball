#include "Gumball.hpp"
#include <fstream>


Engine::Engine() {	
}
Engine::~Engine() {
	glfwTerminate();
}
void Engine::setup() {
	window = new Window;
	window->create("t", 10, 10);

	assetManager = &AssetManager::instance();
	objectManager = &ObjectManager::instance();
	renderManager = &RenderManager::instance();

	assetManager->pushFactory("shader", new ShaderFactory);
	assetManager->pushFactory("mesh", new MeshFactory);
	assetManager->pushFactory("texture", new TextureFactory);
}
void Engine::tick() {
	while (true) {
		objectManager->tick();
		window->clearBuffer();
		renderManager->disposeRender();

		window->swapBuffers();
	}
}