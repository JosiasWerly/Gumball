#ifndef _gumball
#define _gumball
#include "Patterns.hpp"
#include "Renderer.hpp"
#include "Drawables.hpp"
#include "Memory.hpp"
#include "Actor.hpp"

class Engine : 
	public Singleton<Engine>{
public:
	Window *window;//responsible for the window, init gla, ....
	RenderManager *renderManager; //resolve my render pipe....
	AssetManager *assetManager; //responbile for loading assets, also contais all factories
	ObjectManager *objectManager;

	Engine();
	~Engine();

	//THIS IS TEMPORARY
	bool keyPressed(int key) {
		return glfwGetKey(window->getGLFWindow(), key) == GLFW_PRESS;
	}
	bool keyReleased(int key) {
		return glfwGetKey(window->getGLFWindow(), key) == GLFW_RELEASE;
	}

	void tick();
};
static Engine& engine = Engine::instance();

//this is just to make it simplier for now... until i have peace in my heart for supporting multiple renderers && cameras
static Window& window = *engine.window;
static AssetManager& assetManager = *engine.assetManager;
static RenderManager& render = *engine.renderManager;
static ObjectManager& objects = *engine.objectManager;
#endif // !_gumball
