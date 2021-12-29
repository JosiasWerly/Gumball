#include "Engine.hpp"
#include "AssetManager.hpp"

#include "Shaders.hpp"
#include "Drawable.hpp"
#include "Texture.hpp"

using namespace std;


int main() {
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	cout << glGetString(GL_VERSION) << endl;
	glfwMakeContextCurrent(window);

	
	auto &engine = Engine::instance();
	engine.initialize();
	engine.onPlay();

	auto &assetSystem = AssetsSystem::instance();
	assetSystem.loadAllFiles("res\\");


	Texture *tex = nullptr;
	*assetSystem["logo"] >> tex;

	DrawInstance d;
	d.Bind();
	
	int *sh;
	*assetSystem["default"] >> sh;

	glUseProgram(*sh);
	glUniform4f(glGetUniformLocation(*sh, "uColor"), 1, 1, 1, 0);
	glUniform1i(glGetUniformLocation(*sh, "uTexture"), 0);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
		tex->bind();
		d.draw();		
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

/*
* Engine class: Modules
* Windown: Viewport, drawcalls
* Gameplay: Actor, Component, GarbageCollector
* UI
*/

// asset factory
// widown
// Gameplay classes: 
//Actor, Component, GarbageCollector
// UI



