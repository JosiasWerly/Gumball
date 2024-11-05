#include "Window.hpp"
#include "Core/GLUtils.hpp"
#include "Render.module.hpp"

void Window::create(string name, Vector2i size) {
	winSize = size;
	window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw 1;
	}
	glfwMakeContextCurrent(window);
	
	
	glfwSetFramebufferSizeCallback(window,
	   [](GLFWwindow *window, int width, int height) {
			RenderModule::instance()->getWindow().onResize.broadcast(width, height);
	   }
	);
	glfwSetKeyCallback(window,
	   [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		   RenderModule::instance()->getWindow().onInput.broadcast(key, scancode, action, mods);
	   }
	);
}
void Window::clearRender() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::render() {
	glfwPollEvents();
	glfwSwapBuffers(window);
}
void Window::setSize(Vector2i newSize) {
	winSize = newSize;
	glfwSetWindowSize(window, winSize.x, winSize.y);
	glViewport(0, 0, winSize.x, winSize.y);
}
const Vector2i &Window::getSize() {
	return winSize;
}
GLFWwindow *Window::getGLWindow() {
	return window;
}
void Window::setTitle(string newTitle) {
	glfwSetWindowTitle(window, newTitle.c_str());
}