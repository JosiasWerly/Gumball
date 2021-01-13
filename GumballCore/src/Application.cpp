#include "Gumball.hpp"
#include "lang.hpp"



int main() {
	Engine& engine = Engine::instance();
	engine.setup();
	engine.window->config("Gameplay test", 800, 600);
	engine.assetManager->loadAssets("res/");
	engine.tick();
	return 0;
}
/*
##### Gumball 0.1 #####
refator:
[mk3] render and window relation
[mk2] asset system, that can support runtime load
[mk1] project sample
[mk1] separe gamethread vs drawThread && querrySystem

[todo] collision detection
[todo] traces, lines
*/