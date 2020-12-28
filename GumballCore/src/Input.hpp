#ifndef _input
#define _input
#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"


//#define keyWrapper(key) key = int(*#key)
#define keyWrapper(key) key = GLFW_KEY_##key
enum class eKeyboard{
	unknow = -1,
	keyWrapper(A),	keyWrapper(B),	keyWrapper(C), 
	keyWrapper(D), 	keyWrapper(E), 	keyWrapper(F), 
	keyWrapper(G), 	keyWrapper(H), 	keyWrapper(I), 
	keyWrapper(J), 	keyWrapper(K), 	keyWrapper(L), 
	keyWrapper(M), 	keyWrapper(N), 	keyWrapper(O), 
	keyWrapper(P), 	keyWrapper(Q), 	keyWrapper(R), 
	keyWrapper(S), 	keyWrapper(T), 	keyWrapper(U), 
	keyWrapper(V), 	keyWrapper(X), 	keyWrapper(Z), 
	keyWrapper(W), 	keyWrapper(Y),
	
	keyWrapper(UP), keyWrapper(DOWN),
	keyWrapper(RIGHT), keyWrapper(LEFT),
};
enum class eMouse{
	LEFT = 0, 
	RIGHT = 1,
	MIDDLE = 2,
};
#undef keyWrapper

class Input{
public:
	set<int> key_pressedButtons; //
	static void setup(Window focusWin, ) {
		glfwSetKeyCallback(focusWin.getGLFWindow(), );

	}
	static void keyboardCallback(GLFWwindow*, int, int, int, int) {

	}
	static bool isPressed(eKeyboard keycode) {
	}
	static bool isRelease(eKeyboard keycode) {
	}
	static bool pressed(eKeyboard keycode) {
	}
	static bool release(eKeyboard keycode) {
	}
	void tick() {
	}
};

#endif // !_input