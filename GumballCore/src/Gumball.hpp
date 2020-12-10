#ifndef _gumball
#define _gumball
#include "Patterns.hpp"
#include "Renderer.hpp"

class Engine{
public:
	Window *window;//responsible for the window, init gla, ....
	Renderer *render;//manager all drawcalls and camera(s) - for now only 1 camera, depends on window for work
	AssetManager *assetManager; //responbile for loading assets, also contais all factories

	Engine();
	~Engine();

	//THIS IS TEMPORARY
	bool keyPressed(int key) {
		return glfwGetKey(window->getGLFWindow(), key) == GLFW_PRESS;
	}
	bool keyReleased(int key) {
		return glfwGetKey(window->getGLFWindow(), key) == GLFW_RELEASE;
	}
};
static Engine engine;

//this is just to make it simplier for now... until i have peace in my heart for supporting multiple renderers && cameras
static AssetManager& assetManager = *engine.assetManager;
static Window& window = *engine.window;
static Renderer& render = *engine.render;
#endif // !_gumball
