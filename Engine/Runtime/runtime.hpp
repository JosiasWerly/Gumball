#include <Gumball/Core/Engine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>
#include <Gumball/Containers/Pointer.hpp>
Extern {
	Export unsigned int NvOptimusEnablement = 0x00000001;
}

void injectModules(Plugin::Controller *mCtrl);
const char* engineDir();

int main(int argc, char *argv[]) {
	using namespace Engine;
	using namespace Concurrent;
	
	Scheduler *scheduler = nullptr;
	Core core;
	core.Initialize(Core::Init{ argc, argv, engineDir(), scheduler, injectModules });
	scheduler->Initialize(0);
	scheduler->Run();
	return 0;
}