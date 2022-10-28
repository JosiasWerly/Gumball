#include <GumballCore/Engine.hpp>
extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}
int main(int argc, char *argv[]) {	
	Engine::setInstance(new Engine);
	Engine::instance()->args(argc, argv);
	Engine::instance()->tick();
	return 0;
}