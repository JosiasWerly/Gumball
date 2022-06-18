#include "Engine.hpp"
#include <iostream>
#include <string>
#include "Var.hpp"
using namespace std;

class Base {
public:
	string val;
	Base() { val = "BASE"; }
};

class A : public Base{
public:
	A() { val = "A"; }
};

class B : public Base {
public:
	B() { val = "B"; }
};



int main(int argc, char *argv[]) {
	Engine::setInstance(new Engine);
	Engine::inst->args(argc, argv);
	Engine::inst->tick();
	return 0;
}