#include <Gumball/Engine.hpp>
Extern {
	Export unsigned int NvOptimusEnablement = 0x00000001;
}

void injectModules(ModuleController *mCtrl);

int main(int argc, char *argv[]) {
	Engine *engine = new Engine;
	engine->initialize({ argc, argv, injectModules });
	engine->tick();
	return 0;
}