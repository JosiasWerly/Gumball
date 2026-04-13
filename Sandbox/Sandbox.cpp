#include "Sandbox.hpp"
#include <Gumball/Core/Engine.hpp>
#include <Gumball/Concurrent/Scheduler.hpp>
#include <Gumball/Resource/Controller.hpp>
#include <iostream>

using namespace std;
using namespace Engine;

void MyProject::Attached() {
	cout << "loaded" << endl;
	Engine::Core *c = Engine::Core::Instance();
	
	cout << endl;
}
void MyProject::Detached() {
	cout << "unloaded" << endl;
}