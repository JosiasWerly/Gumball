#include "Input.hpp"
#include "Renderer.hpp"
#include <iostream>

GLFWwindow* Input::currentWin;
map<eKeyboard, eAction> Input::keysState, Input::pooledKeys;
bool Input::containKey(map<eKeyboard, eAction>& map, eKeyboard key) {
	return map.find(key) != map.end();
}
bool Input::checkAction(map<eKeyboard, eAction>& map, eKeyboard key, eAction toCheck) {
	auto k = map.find(key);
	if (k != map.end())
		return (*k).second == toCheck;
	return false;
}
void Input::setFocus(class Window& focusWin) {
	Input::currentWin = focusWin.getGLFWindow();
}
void Input::poolEvents() {
	constexpr int eMax = sizeof(eKeyboard);
	/*keysState = pooledKeys;
	for (map<eKeyboard, eAction>::iterator it = keysState.begin(); it != keysState.end();){		
		if ((*it).second == eAction::RELEASED) {
			keysState.erase(it++);
			continue;
		}
		else if((*it).second == eAction::PRESSED) {
			(*it).second = eAction::REPEAT;
		}
		++it;
	}*/
}
bool Input::isKeyDown(eKeyboard key) {
	return glfwGetKey(Input::currentWin, (int)key) == GLFW_PRESS;
}
bool Input::onPressed(eKeyboard key) {
	return false;
}
bool Input::onRelease(eKeyboard key) {
	return false;
}