#include "Sandbox.hpp"
#include <Gumball/Core/Engine.hpp>
#include <Gumball/Resource/Controller.hpp>
#include <iostream>

using namespace std;
using namespace Core;

void MyProject::Attached() {
	cout << "loaded" << endl;
	Core::Engine &e = Core::Engine::Instance();
	
	cout << endl;
}
void MyProject::Detached() {
	cout << "unloaded" << endl;
}