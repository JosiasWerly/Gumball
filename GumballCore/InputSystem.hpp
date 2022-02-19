#pragma once
#ifndef _inputsystem
#define _inputsystem
#include <map>
#include <unordered_map>
#include <list>

#include "GLUtils.hpp"
#include "Patterns.hpp"
#include "EngineSystem.hpp"
#include "Event.hpp"


#define keyWrapper(key) key = GLFW_KEY_##key
namespace Input {
	enum class EKeyCode {
		unknow = -1,
		keyWrapper(A), keyWrapper(B), keyWrapper(C),
		keyWrapper(D), keyWrapper(E), keyWrapper(F),
		keyWrapper(G), keyWrapper(H), keyWrapper(I),
		keyWrapper(J), keyWrapper(K), keyWrapper(L),
		keyWrapper(M), keyWrapper(N), keyWrapper(O),
		keyWrapper(P), keyWrapper(Q), keyWrapper(R),
		keyWrapper(S), keyWrapper(T), keyWrapper(U),
		keyWrapper(V), keyWrapper(X), keyWrapper(Z),
		keyWrapper(W), keyWrapper(Y),

		keyWrapper(UP), keyWrapper(DOWN),
		keyWrapper(RIGHT), keyWrapper(LEFT),
	};
	enum class EMouseCode {
		left = 0,
		right = 1,
		middle = 2,
		spec1 = 3,
		spec2 = 4,
	};
	enum class EActionType {
		pressed = GLFW_PRESS,
		released = GLFW_RELEASE,
		repeat = GLFW_REPEAT,
	};

	enum class EventType {
		keyboard, mouse
	};

	struct Event {
		EActionType actionType;
		EventType eventType;

		union {
			EKeyCode keycode;
			EMouseCode mouseCode;
		};
	};
};

class InputSystem : 
	public IEngineSystem {
public:
	struct KeyCodeStatus {
		bool pressed, released, repeat; 
	};	
	
	std::map<Input::EKeyCode, KeyCodeStatus> keysStatus, keyPool;
	EventPool<Input::Event> eventPool;

	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	bool isKeyDown(Input::EKeyCode Key);
	bool onKeyPressed(Input::EKeyCode Key);
	bool onKeyReleased(Input::EKeyCode Key);

	void tick(float deltaTime) override;
};

#undef keyWrapper
#endif // !_transform
