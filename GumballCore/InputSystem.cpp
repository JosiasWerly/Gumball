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
		auto& newKeyStatus = kv.second;
		auto& targetKeyStatus = keysStatus[kv.first];

		if (newKeyStatus.pressed) {
			targetKeyStatus.pressed = true;
		}
		else if (newKeyStatus.released) {
			targetKeyStatus.released = true;
		}
		else if (targetKeyStatus.released && !newKeyStatus.released)
			targetKeyStatus.released = targetKeyStatus.pressed = false;

		newKeyStatus.pressed = newKeyStatus.released = false;
	}
}

bool InputSystem::isKeyDown(Input::EKeyCode Key) {
	const auto &KeyStatus = keysStatus[Key];
	return KeyStatus.pressed && !KeyStatus.released;
}
//this is broken TODO: add new KeyStatus::isRepeating
bool InputSystem::onKeyPressed(Input::EKeyCode Key) {
	const auto& KeyStatus = keysStatus[Key];
	return KeyStatus.pressed && KeyStatus.released;
}
bool InputSystem::onKeyReleased(Input::EKeyCode Key) {
	const auto& KeyStatus = keysStatus[Key];
	return KeyStatus.pressed && KeyStatus.released;
}

