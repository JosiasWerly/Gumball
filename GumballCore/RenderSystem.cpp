#include "RenderSystem.hpp"


void Window::create(string Name, Vector2i size) {
	winSize = size;
	window = glfwCreateWindow(size.x, size.y, Name.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw 1;
	}
	glfwMakeContextCurrent(window);	
}
void Window::clearRender() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::render() {
	glfwPollEvents();
	glfwSwapBuffers(window);
	msCounter.tick();
}
void Window::setSize(Vector2i newSize) {
	winSize = newSize;
	glfwSetWindowSize(window, winSize.x, winSize.y);
	glViewport(0, 0, winSize.x, winSize.y);
}
const Vector2i& Window::getSize() {
	return winSize;
}
GLFWwindow* Window::getWindowReference() {
	return window;
}
const double& Window::getMS() {
	return msCounter.getMS();
}

void RenderSystem::initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mainWindow.create("Gumball", { 800, 600 });
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw 1;
	}
}
void RenderSystem::shutdown() {
	
	glfwTerminate();
}
void RenderSystem::tick() {
	mainWindow.clearRender();

	for (auto& view : drawboard.views)
	{
		auto viewMat = view->transform.getMat();
		for (auto& draw : drawboard.drawInstances)
		{
			draw->material.setParameter<glm::mat4>("uView", viewMat);
			draw->material.setParameter<glm::mat4>("uProj", view->viewMode.mProjection);
			draw->material.setParameter<glm::mat4>("uModel", draw->transform.getMat());
			draw->draw();
		}
	}

	//TODO: LAYER
	//for (auto &kv : drawboards)
	//{
	//	auto& drawboard = kv.second;
	//	for (auto &view : drawboard.views)
	//	{
	//		auto viewMat = view->transform.getMat();
	//		for (auto& draw : drawboard.drawInstances)
	//		{
	//			draw->material.setParameter<glm::mat4>("uView", viewMat);
	//			draw->material.setParameter<glm::mat4>("uProj", view->viewMode.mProjection);
	//			draw->material.setParameter<glm::mat4>("uModel", draw->transform.getMat());
	//		}
	//	}
	//}

	//d->draw();
	mainWindow.render();
}

