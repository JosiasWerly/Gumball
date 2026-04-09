#include <Gumball/Engine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>
#include <Gumball/Containers/Pointer.hpp>
Extern {
	Export unsigned int NvOptimusEnablement = 0x00000001;
}

void injectModules(Plugin::Controller *mCtrl);
const char* engineDir();

int main(int argc, char *argv[]) {
	Concurrent::Scheduler scheduler;
	scheduler.Initialize(0);
	
	Engine *engine = new Engine;
	engine->initialize({ argc, argv, engineDir(), &scheduler, injectModules });
	
	scheduler.Run();
	return 0;
}