#pragma once
#ifndef __window
#define __window

#include <string>
using namespace std;

class GMODULE Window {
	struct GLFWwindow *window = nullptr;
	Vector2i winSize;

public:
	void create(string name, Vector2i size);
	void clearRender();
	void render();
	void setSize(Vector2i size);
	const Vector2i &getSize();
	struct GLFWwindow *getGLWindow();
	void setTitle(string newTitle);

	Event<void(int width, int height)> onResize;
	Event<void(int key, int scancode, int action, int mods)> onInput;
};

#endif // !__window