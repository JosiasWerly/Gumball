#ifndef _gumball
#define _gumball

#include "Patterns.hpp"
#include "Drawables.hpp"
#include "Actor.hpp"
#include "Script.hpp"


#include "RenderManager.hpp"
#include "ObjectManager.hpp"
#include "InputManager.hpp"
#include "AssetManager.hpp"
#include "dllLoader/dllLoader.hpp"


struct iProject{
	virtual void setup() {

	}
	virtual void tick() {

	}
};
class Engine : 
	public Singleton<Engine>{
public:
	iProject* proj;
	Window *window;//responsible for the window, init gla, ....
	RenderManager *renderManager; //resolve my render pipe....
	AssetManager *assetManager; //responbile for loading assets, also contais all factories
	ObjectManager *objectManager;
	ProjectManager projectManager;


	Engine();
	~Engine();

	void setup();
	void tick();
};
//this is just to make it simplier for now... until i have peace in my heart for supporting multiple renderers && cameras
//static Engine& engine = Engine::instance();
#endif // !_gumball
