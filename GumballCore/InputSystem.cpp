#include "InputSystem.hpp"

using namespace Input;

std::map<Input::EKeyCode, InputSystem::KeyCodeStatus> InputSystem::keyPool, InputSystem::keysStatus;

//just need to capture when the state changed
void InputSystem::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	const EKeyCode keyCode = static_cast<EKeyCode>(key);
	const EType type = static_cast<EType>(action);
	if (type == EType::repeat)
		return;
	if(type == EType::pressed)
		keyPool[keyCode].pressed = true;
	else
		keyPool[keyCode].released = true;
}
void InputSystem::processInputs() {
	for (auto &kv : keyPool) {
		auto& nKeyStatus = kv.second;
		auto& tKeyStatus = keysStatus[kv.first];

		if (nKeyStatus.released) {
			tKeyStatus.pressed = false;
			tKeyStatus.repeat = false;
			tKeyStatus.released = true;
		}
		else if (nKeyStatus.pressed) {
			tKeyStatus.pressed = true;			
		}
		else if (tKeyStatus.pressed) {
			tKeyStatus.pressed = false;
			tKeyStatus.repeat = true;
		}
		else if (tKeyStatus.released) {
			tKeyStatus.released = false;
		}
		nKeyStatus.released = nKeyStatus.pressed = false;
	}
}

bool InputSystem::isKeyDown(Input::EKeyCode Key) {
	const auto &KeyStatus = keysStatus[Key];
	return KeyStatus.repeat || KeyStatus.pressed;
}
bool InputSystem::onKeyPressed(Input::EKeyCode Key) {
	const auto& KeyStatus = keysStatus[Key];
	return KeyStatus.pressed;
}
bool InputSystem::onKeyReleased(Input::EKeyCode Key) {
	const auto& KeyStatus = keysStatus[Key];
	return KeyStatus.released;
}

