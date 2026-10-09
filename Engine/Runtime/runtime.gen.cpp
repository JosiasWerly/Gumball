#include "Runtime.hpp"
#include <Gumball/Plugin/Controller.hpp>
#include <Gumball/Plugin/Module.hpp>

void Wait(Concurrent::ATask &tsk) {
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
class MyModule : 
	public ::Core::Global<MyModule>,
	public Plugin::IModule {

public:
	Concurrent::Task Load() { 
		Concurrent::Task tk;
		tk.Run().Bind(&Wait);
		return tk;
	}
	const char *Name() const { return "MyModule"; };
};
void injectModules(Plugin::Controller *ctrl) {
	ctrl->Add<MyModule>();
}
const char* engineDir() { return "C:\\Users\\josia\\source\\repos\\JosiasWerly\\Gumball\\"; }
