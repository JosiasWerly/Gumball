#pragma once
#ifndef __window
#define __window

#include <string>
#include <Gumball/Event.hpp>
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

	Dispatcher<int, int> onResize;
	Dispatcher<int, int, int, int> onInput;
};

#endif // !__window