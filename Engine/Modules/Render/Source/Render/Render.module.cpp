#include "Render.module.hpp"

#include "Core/GLUtils.hpp"
#include "Scene/SceneOverlay.hpp"
#include "Widget/WidgetOverlay.hpp"
#include "Core/Mesh.hpp"
#include "Core/Shaders.hpp"

#include <Asset/Asset.module.hpp>

RenderModule::RenderModule() :
	scene(new SceneOverlay),
	widget(new WidgetOverlay) {
}
RenderModule::~RenderModule() {
	for (auto &l : overlays)
		delete l;
}
void RenderModule::load() {
	AssetModule *content = AssetModule::instance();
	content->addBuilder(new WAssetBuilder<MeshData>);
	content->addBuilder(new WAssetBuilder<Shader>);
	content->addBuilder(new WAssetBuilder<Texture>);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);


	mainWindow.create("Gumball", { 800, 600 });
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
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}
void RenderModule::unload() {
	glfwTerminate();
}
void RenderModule::posLoad() {
	addOverlay(scene);
	addOverlay(widget);
}
void RenderModule::tick(const double &deltaTime) {
	mainWindow.clearRender();

	for (RenderOverlay *layer : overlays)
		layer->onRender(deltaTime);
	
	mainWindow.render();
}
void RenderModule::addOverlay(RenderOverlay *overlay, bool front){
	if (front)
		overlays.emplace_front(overlay);
	else
		overlays.emplace_back(overlay);
	overlay->_window = &mainWindow;
	overlay->onAttach();
}
void RenderModule::remOverlay(RenderOverlay *overlay) {
	overlay->onDetach();
	overlays.remove(overlay);
}
RenderOverlay *RenderModule::getOverlay(string name) {
	for (RenderOverlay *layer : overlays) {
		if (layer->name() == name)
			return layer;
	}
	return nullptr;
}
//void RenderModule::onWindowResize(GLFWwindow *window, int width, int height) {
//	//auto RSys = Engine::instance()->renderSystem;
//	//RSys->mainWindow.setSize(Vector2i(width, height));
//}