#include "InputSystem.hpp"
#include "Engine.hpp"
using namespace Input;

//std::map<Input::EKeyCode, InputSystem::KeyCodeStatus> InputSystem::keyPool, InputSystem::keysStatus;
//EventPool<Input::Event> InputSystem::eventPool;

//just need to capture when the state changed
void InputSystem::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//REMOVE: input system disable for the time being
	return;
	static auto &inputSys = *Engine::instance()->getSystem<InputSystem>();
	const EKeyCode keyCode = static_cast<EKeyCode>(key);
	const EActionType type = static_cast<EActionType>(action);

	if (type == EActionType::repeat)
		return;
	if(type == EActionType::pressed)
		inputSys.keyPool[keyCode].pressed = true;
	else
		inputSys.keyPool[keyCode].released = true;
}
void InputSystem::tick(float deltaTime) {
	eventPool.clearPool();
	Input::Event e;
	for (auto& kv : keyPool) {
		auto& nKeyStatus = kv.second;
		auto& tKeyStatus = keysStatus[kv.first];


		if (nKeyStatus.released) { //onRelease
			tKeyStatus.pressed = false;
			tKeyStatus.repeat = false;
			tKeyStatus.released = true;

			e.actionType = EActionType::released;
			e.eventType = EventType::keyboard;
			e.keycode = kv.first;
			eventPool << e;
		}
		else if (nKeyStatus.pressed) { //onPressed
			tKeyStatus.pressed = true;

			e.actionType = EActionType::pressed;
			e.eventType = EventType::keyboard;
			e.keycode = kv.first;
			eventPool << e;
		}
		else if (tKeyStatus.pressed) { //onRepeat
			tKeyStatus.pressed = false;
			tKeyStatus.repeat = true;

			e.actionType = EActionType::repeat;
			e.eventType = EventType::keyboard;
			e.keycode = kv.first;
			eventPool << e;
		}
		else if (tKeyStatus.released) { //release states
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
