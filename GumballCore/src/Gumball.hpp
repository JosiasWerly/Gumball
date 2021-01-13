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


class Engine : 
	public Singleton<Engine>{
public:
	Window *window;//responsible for the window, init gla, ....
	RenderManager *renderManager; //resolve my render pipe....
	AssetManager *assetManager; //responbile for loading assets, also contais all factories
	ObjectManager *objectManager;
	ProjectManager project;
	
	Engine();
	~Engine();

	void setup();
	void tick();
};
#endif // !_gumball
