#ifndef _gumball
#define _gumball


class Engine{
public:
	//class Window* window;//responsible for the window, init gla, ....
	//class Renderer* render;//manager all drawcalls and camera(s) - for now only 1 camera, depends on window for work
	//class AssetManager* assetManager; //responbile for loading assets, also contais all factories

	Engine();
	~Engine();
};
#endif // !_gumball
