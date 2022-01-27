#include "RenderSystem.hpp"
#include "EditorOverlay.hpp"
#include "Engine.hpp"



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
}
void Window::setSize(Vector2i newSize) {
	winSize = newSize;
	glfwSetWindowSize(window, winSize.x, winSize.y);
	glViewport(0, 0, winSize.x, winSize.y);
}
const Vector2i& Window::getSize() {
	return winSize;
}
GLFWwindow* Window::GetGLWindow() {
	return window;
}


void IRenderOverlay::onRender(float deltaTime) {
	for (auto& view : views) {
		auto viewMat = view->transform.getMat();
		for (auto& draw : drawInstances) {
			//draw->material.setParameter<glm::mat4>("uView", viewMat);
			//draw->material.setParameter<glm::mat4>("uProj", view->viewMode.mProjection);
			//draw->material.setParameter<glm::mat4>("uModel", draw->transform.getMat());

			draw->material.setParameter<glm::mat4>("uView", viewMat);
			draw->material.setParameter<glm::mat4>("uProj", view->viewMode.mProjection);
			draw->material.setParameter<glm::mat4>("uModel", draw->transform.getMat());
			draw->draw();
		}
	}
}

void RenderSystem::initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	cout << "glVersion" << glfwGetVersionString() << endl;

	mainWindow.create("Gumball", { 800, 600 });
	
	glfwSetKeyCallback(mainWindow.GetGLWindow(), &InputSystem::keyboardCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw 1;
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.1f, 100.0f);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	// Cull triangles which normal is not towards the camera
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	pushLayer(new IRenderOverlay("game"));
	pushLayer(new EditorOverlay);
}
void RenderSystem::shutdown() {
	
	glfwTerminate();
}
void RenderSystem::tick() {
	mainWindow.clearRender();

	for (auto &layer : layers)
		layer->onRender(0);
	
	mainWindow.render();
}