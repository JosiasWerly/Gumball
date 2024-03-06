#include "RenderSystem.hpp"
#include "SceneOverlay.hpp"
#include "WidgetOverlay.hpp"
#include "Engine.hpp"

void Window::create(string name, Vector2i size) {
	winSize = size;
	window = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL);
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
void Window::setTitle(string newTitle) {
	glfwSetWindowTitle(window, newTitle.c_str());
}

void RenderOverlay::onAttach() {}
void RenderOverlay::onDetach() {}
void RenderOverlay::onRender(const double &deltaTime) {}

RenderSystem::RenderSystem() :
	scene(new SceneOverlay),
	widget(new WidgetOverlay) {
}
RenderSystem::~RenderSystem() {
	for (auto &l : overlays)
		delete l;
}
void RenderSystem::initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);


	mainWindow.create("Gumball", { 800, 600 });
	
	glfwSetFramebufferSizeCallback(mainWindow.getGLWindow(), &RenderSystem::onWindowResize);
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
}
void RenderSystem::lateInitialize() {
	pushOverlay(scene);
	pushOverlay(widget);
}
void RenderSystem::shutdown() {	
	glfwTerminate();
}
void RenderSystem::tick(const double &deltaTime) {
	mainWindow.clearRender();

	for (auto &layer : overlays)
		layer->onRender(deltaTime);
	
	mainWindow.render();
}
void RenderSystem::pushOverlay(RenderOverlay *overlay, bool front){
	if (front)
		overlays.emplace_front(overlay);
	else
		overlays.emplace_back(overlay);
	overlay->onAttach();
}
void RenderSystem::popOverlay(RenderOverlay *overlay) {
	overlay->onDetach();
	overlays.remove(overlay);
}
RenderOverlay *RenderSystem::getOverlay(string name) {
	for (auto &l : overlays) {
		if (l->name == name)
			return l;
	}
	return nullptr;
}
void RenderSystem::onWindowResize(GLFWwindow *window, int width, int height) {
	auto RSys = Engine::instance()->renderSystem;
	RSys->mainWindow.setSize(Vector2i(width, height));
}