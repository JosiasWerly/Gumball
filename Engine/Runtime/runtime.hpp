#include <Gumball/Engine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>
#include <Gumball/Containers/Pointer.hpp>
Extern {
	Export unsigned int NvOptimusEnablement = 0x00000001;
}

void injectModules(Plugin::Controller *mCtrl);
const char* engineDir();

struct SuperFoo {};
struct Foo : public SuperFoo {};
struct SubFoo : public Foo {};

int main(int argc, char *argv[]) {
	Concurrent::Scheduler scheduler;
	scheduler.Start(1);	
	
	Engine *engine = new Engine;
	engine->initialize({ argc, argv, engineDir(), &scheduler, injectModules });
	
	scheduler.RunMainThread();
	return 0;
}