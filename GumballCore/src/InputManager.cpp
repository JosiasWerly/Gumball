#include "InputManager.hpp"
#include "RenderManager.hpp"
#include <iostream>
#include <map>

//this is just the craziest input system that i ever made... plx joe from the future apologies.
Input::map Input::poolKeystate, Input::keysToEvaluate;
bool Input::checkAction(map& map, eKeyboard key, eAction action) {
	auto k = map.find(key);
	if (k != map.end()) {
		switch (action)	{
			case eAction::PRESSED:			return (*k).second.pressed;
			case eAction::RELEASED:			return (*k).second.released;
			case eAction::REPEAT:			return (*k).second.repeating;
		}
		return false;
	}
	return false;
}
void Input::setFocus(class Window& focusWin) {
	glfwSetKeyCallback(focusWin.getGLFWindow(), Input::keyboardCallback);
}
void Input::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	eKeyboard k = (eKeyboard)key;
	eAction a = (eAction)action;
	if(a == eAction::PRESSED || a == eAction::REPEAT)
		poolKeystate[k].pressed = true;
	else if(a == eAction::RELEASED)
		poolKeystate[k].released = true;
}
void Input::poolEvents() {
	list<eKeyboard> toRemove;
	for (auto k : keysToEvaluate) {
		if (k.second.released) {
			toRemove.push_back(k.first);
		}
		else if (k.second.pressed) {
			k.second.pressed = false;
			k.second.repeating = true;
			keysToEvaluate[k.first] = k.second;
		}
	}
	for (auto k : toRemove) {
		keysToEvaluate.erase(k);
		poolKeystate.erase(k);
	}

	for (auto pk : poolKeystate) {
		if (pk.second.released)
			keysToEvaluate[pk.first].released = true;
		if (keysToEvaluate.find(pk.first) == keysToEvaluate.end())
			keysToEvaluate[pk.first] = pk.second;
	}

}

bool Input::isKeyDown(eKeyboard key) {
	return checkAction(keysToEvaluate, key, eAction::PRESSED) || checkAction(keysToEvaluate, key, eAction::REPEAT);
}
bool Input::onPressed(eKeyboard key) {
	return checkAction(keysToEvaluate, key, eAction::PRESSED);
}
bool Input::onRelease(eKeyboard key) {
	return checkAction(keysToEvaluate, key, eAction::RELEASED);
}