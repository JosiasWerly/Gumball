#include "Input.module.hpp"
#include <Render/Render.module.hpp>

using namespace Input;


bool Input::isKeyDown(Input::EKeyCode Key) {
	const auto &KeyStatus = InputModule::instance()->keysStatus[Key];
	return KeyStatus.repeat || KeyStatus.pressed;
}
bool Input::onKeyPressed(Input::EKeyCode Key) {
	const auto &KeyStatus = InputModule::instance()->keysStatus[Key];
	return KeyStatus.pressed;
}
bool Input::onKeyReleased(Input::EKeyCode Key) {
	const auto &KeyStatus = InputModule::instance()->keysStatus[Key];
	return KeyStatus.released;
}


EventPool &EventPool::operator<<(Input::Event data) {
	events.push_back(data);
	return *this;
}
bool EventPool::operator>>(Input::Event &data) {
	if (hasEvents()) {
		data = events.back();
		events.pop_back();
		return true;
	}
	return false;
}

void InputModule::onInputCallback(int key, int scancode, int action, int mods) {
	const EKeyCode keyCode = static_cast<EKeyCode>(key);
	const EActionType type = static_cast<EActionType>(action);

	if (type == EActionType::repeat)
		return;
	if (type == EActionType::pressed)
		keyPool[keyCode].pressed = true;
	else
		keyPool[keyCode].released = true;
}
void InputModule::posLoad() {
	RenderModule::instance()->getWindow().onInput.bind(this, &InputModule::onInputCallback);
}
void InputModule::tick(const double &deltaTime) {
	eventPool.clearPool();
	Input::Event e;
	for (auto &kv : keyPool) {
		auto &nKeyStatus = kv.second;
		auto &tKeyStatus = keysStatus[kv.first];


		if (nKeyStatus.released) { //onRelease
			tKeyStatus.pressed = false;
			tKeyStatus.repeat = false;
			tKeyStatus.released = true;

			e.actionType = EActionType::released;
			e.eventType = EInputDevice::keyboard;
			e.keycode = kv.first;
			eventPool << e;
		}
		else if (nKeyStatus.pressed) { //onPressed
			tKeyStatus.pressed = true;

			e.actionType = EActionType::pressed;
			e.eventType = EInputDevice::keyboard;
			e.keycode = kv.first;
			eventPool << e;
		}
		else if (tKeyStatus.pressed) { //onRepeat
			tKeyStatus.pressed = false;
			tKeyStatus.repeat = true;

			e.actionType = EActionType::repeat;
			e.eventType = EInputDevice::keyboard;
			e.keycode = kv.first;
			eventPool << e;
		}
		else if (tKeyStatus.released) { //release states
			tKeyStatus.released = false;
		}
		nKeyStatus.released = nKeyStatus.pressed = false;
	}
}