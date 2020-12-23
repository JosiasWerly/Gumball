#include "Gumball.hpp"
#include "Renderer.hpp"


Engine::Engine() {
	window = new Window;
	assetManager = &AssetManager::instance();
	memoryManager = &ObjectManager::instance();
	renderManager = &RenderManager::instance();

	renderManager->currentContext = new Renderer(window);

	assetManager->pushFactory("shader", new ShaderFactory);
	assetManager->pushFactory("mesh", new MeshFactory);
	assetManager->pushFactory("texture", new TextureFactory);
}
Engine::~Engine() {
	glfwTerminate();
}


