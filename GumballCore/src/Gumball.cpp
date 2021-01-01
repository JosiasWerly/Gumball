#include "Gumball.hpp"
#include "Renderer.hpp"


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
	renderManager->render.attachWindow(window);

	assetManager->pushFactory("shader", new ShaderFactory);
	assetManager->pushFactory("mesh", new MeshFactory);
	assetManager->pushFactory("texture", new TextureFactory);
}
void Engine::tick() {
	objectManager->tick();
	window->clearBuffer();
	renderManager->disposeRender();
	window->swapBuffers();
}