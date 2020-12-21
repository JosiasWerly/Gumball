#include "Gumball.hpp"
#include "Renderer.hpp"


Engine::Engine() {
	render = new Renderer;
	window = new Window;
	window->attachRender(render);
	assetManager = &AssetManager::instance();
	memoryManager = &ExecutronManager::instance();

	assetManager->pushFactory("shader", new ShaderFactory);
	assetManager->pushFactory("mesh", new MeshFactory);
	assetManager->pushFactory("texture", new TextureFactory);
}
Engine::~Engine() {
	glfwTerminate();
}


