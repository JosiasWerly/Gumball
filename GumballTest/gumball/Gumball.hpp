#ifndef _gumball
#define _gumball
#include "Patterns.hpp"
#include "Renderer.hpp"
#include "Drawables.hpp"
#include "Memory.hpp"
#include "Input.hpp"
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

	void setup();
	void tick();
};
//this is just to make it simplier for now... until i have peace in my heart for supporting multiple renderers && cameras
//static Engine& engine = Engine::instance();
#endif // !_gumball
