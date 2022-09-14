#include <GumballCore/Engine.hpp>
int main(int argc, char *argv[]) {	
	Engine::setInstance(new Engine);
	Engine::instance()->args(argc, argv);
	Engine::instance()->tick();
	return 0;
}