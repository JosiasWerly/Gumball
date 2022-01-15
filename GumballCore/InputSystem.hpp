#pragma once
#ifndef _inputsystem
#define _inputsystem
#include "GLUtils.hpp"
#include "Patterns.hpp"
#include <map>
#include <unordered_map>
#include <list>


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
	enum class EType {
		pressed = GLFW_PRESS,
		released = GLFW_RELEASE,
		repeat = GLFW_REPEAT,
	};
};



//TODO: for now there is only one window, so this can be static
class InputSystem {
public:
	struct KeyCodeStatus {
		bool pressed, released; 
	};	
	static std::map<Input::EKeyCode, KeyCodeStatus> keysStatus, keyPool;


	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void processInputs();

	static bool isKeyDown(Input::EKeyCode Key);
	static bool onKeyPressed(Input::EKeyCode Key);
	static bool onKeyReleased(Input::EKeyCode Key);
};


#undef keyWrapper
#endif // !_transform
