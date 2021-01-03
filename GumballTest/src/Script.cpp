#include <iostream>
#include "Script.hpp"
using namespace std;
void BaseScript::test() {
	cout << "this is the base Class" << endl;
}
void ChildScript::test() {
	cout << "ChildScript" << endl;
}
double add(double a, double b) {
	return a + b;
}
BaseScript* createBaseScript() {
	return new ChildScript();
}
