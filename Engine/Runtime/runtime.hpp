#include <Gumball/Core/Engine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>
Extern {
	Export unsigned int NvOptimusEnablement = 0x00000001;
}

void injectModules(Plugin::Controller *mCtrl);
const char* engineDir();

int main(int argc, char *argv[]) {
	using namespace Core;
	using namespace Concurrent;
	
	Scheduler *scheduler = nullptr;
	Engine g;
	g.Initialize(Engine::Init{ argc, argv, engineDir(), scheduler, injectModules });
	scheduler->Initialize(1);
	scheduler->ProducerTick();
	return 0;
}