#include "RenderSystem.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
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
GLFWwindow* Window::getGLWindow() {
	return window;
}


void IRenderOverlay::onAttach() {}
void IRenderOverlay::onDetach() {}
void IRenderOverlay::onRender(float deltaTime) {}

void RenderSystem::initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);


	mainWindow.create("Gumball", { 800, 600 });
	
	glfwSetKeyCallback(mainWindow.getGLWindow(), &InputSystem::keyboardCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw 1;
	}
	
	cout << "glfwVersion " << glfwGetVersionString() << endl;
	cout << "glVersion " << glGetString(GL_VERSION) << endl;

	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.1f, 100.0f);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	// Cull triangles which normal is not towards the camera
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	pushLayer(new SceneOverlay);
	pushLayer(new WidgetOverlay);
}
void RenderSystem::shutdown() {
	
	glfwTerminate();
}
void RenderSystem::pushLayer(IRenderOverlay *layer, bool pushBack) {
	if (pushBack)
		layers.emplace_back(layer);
	else
		layers.emplace_front(layer);
	layer->onAttach();
}
void RenderSystem::popLayer(IRenderOverlay *layer) {
	layer->onDetach();
	layers.remove(layer);
}
void RenderSystem::tick(float deltaTime) {
	mainWindow.clearRender();

	for (auto &layer : layers)
		layer->onRender(deltaTime);
	
	mainWindow.render();
}
IRenderOverlay *RenderSystem::getLayer(string name) {
	for (auto &l : layers) {
		if (l->name == name)
			return l;
	}
	return nullptr;
}
list<IRenderOverlay *> &RenderSystem::getLayerList() { return layers; }