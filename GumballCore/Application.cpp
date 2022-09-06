#include "Engine.hpp"
#include <iostream>
#include <string>
#include "Var.hpp"
using namespace std;

int main(int argc, char *argv[]) {
	Engine::setInstance(new Engine);
	Engine::inst->args(argc, argv);
	Engine::inst->tick();
	return 0;
}