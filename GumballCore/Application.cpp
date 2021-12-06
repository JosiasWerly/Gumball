

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shaders.hpp"
#include "Utils.hpp"
#include "AssetManager.hpp"
#include "Engine.hpp"
#include "Drawable.hpp"

using namespace std;



int main() {
	auto &engine = Engine::instance();
	engine.Initialize();
	
	//AssetsSystem *asset = engine.getSystem<AssetsSystem>();
	
	//engine.OnPlay();
	

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
	
	loadShaders("res\\");

	
	DrawInstance d;





	auto sh = shaders["default"];
	glUseProgram(sh);
	glUniform4f(glGetUniformLocation(sh, "color"), 0, 1, 0, 0);
	glUniform4f(glGetUniformLocation(sh, "color"), 0, 1, 0, 0);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		d.draw();

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(pos));
		//glDrawElements(GL_TRIANGLES, sizeof(id), GL_UNSIGNED_INT, nullptr);

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();



	


	return 0;
}

/*
* Engine class: Modules
* Asset Factory: shader
* Windown: Viewport, drawcalls
* Gameplay: Actor, Component, GarbageCollector
* UI
*/

// asset factory
// widown
// Gameplay classes: 
//Actor, Component, GarbageCollector
// UI



