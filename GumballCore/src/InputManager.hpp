#ifndef _inputManager
#define _inputManager
#include "GLBuffers.hpp"
#include "GFunctionLibrary.hpp"
#include <map>
using namespace std;
//TODO: decent Input manager2

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
//constexpr unsigned int TEST_START_LINE = __LINE__;
//constexpr unsigned int eKeyboardSize = __LINE__ - TEST_START_LINE - 3;

enum class eMouse{
	LEFT = 0, 
	RIGHT = 1,
	MIDDLE = 2,
};
enum class eAction {
	PRESSED = GLFW_PRESS,
	RELEASED = GLFW_RELEASE,
	REPEAT = GLFW_REPEAT,
};
#undef keyWrapper

class Window;
class Input{
public:
	struct sAction{
		bool pressed, released, repeating;
	};
	typedef map<eKeyboard, sAction> map;
	static map poolKeystate, keysToEvaluate;

	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);	
	static void setFocus(class Window& focusWin);
	static void poolEvents();

	static bool checkAction(map& map, eKeyboard key, eAction action);
	static bool isKeyDown(eKeyboard key);
	static bool onPressed(eKeyboard key);
	static bool onRelease(eKeyboard key);
};
#endif // !_inputManager