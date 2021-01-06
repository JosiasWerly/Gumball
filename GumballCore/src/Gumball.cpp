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

	assetManager->pushFactory("shader", new ShaderFactory);
	assetManager->pushFactory("mesh", new MeshFactory);
	assetManager->pushFactory("texture", new TextureFactory);


	//engine.projectManager.attach("C:\\Users\\josia\\Desktop\\Bard\\Gumball\\Build\\Debug\\x64\\GumballTest\\GumballTest.dll");
}
void Engine::tick() {
	//projectManager.currentProject.beginPlay();
	proj->setup();
	while (true) {
		//projectManager.currentProject.tick();
		proj->tick();
		objectManager->tick();
		window->clearBuffer();
		renderManager->disposeRender();
		window->swapBuffers();
	}
}