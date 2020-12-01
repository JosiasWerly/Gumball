#include "Gumball.hpp"
#include "Renderer.hpp"


Engine::Engine() {
	window = new Window;
	render = new Renderer(window);
	assetManager = &AssetManager::instance();

	assetManager->pushFactory("shader", new ShaderFactory);
	assetManager->pushFactory("mesh", new MeshFactory);
	assetManager->pushFactory("texture", new TextureFactory);
}
Engine::~Engine() {
}