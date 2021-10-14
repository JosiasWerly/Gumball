#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shaders.hpp"

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
	
	loadShaders("res\\");
	//glGenBuffers(1, &i);
	//glBindBuffer(GL_ARRAY_BUFFER, i);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, pos, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);




	float pos[] = {
		0, 0, 0,
		0.1, 0.0, 0,
		0.1, 0.1, 0.0,
		0, 0.1, 0.0
	};
	unsigned id[] = {
		0, 1, 2,
		2, 3
	};



	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);



	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);


	unsigned ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(id), id, GL_STATIC_DRAW);



	//float pos[] = {
	//-0.1, -0.1, 0.0,
	//0.0, 0.0, 0.1,
	//0.1, -0.1, 0.0
	//};
	//
	//unsigned vbo;
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	//
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glEnableVertexAttribArray(0);


	auto sh = shaders["default"];
	glUseProgram(sh);


	
	glVertexAttrib1f(glGetAttribLocation(sh, "test"), 0.5);
	glUniform4f(glGetUniformLocation(sh, "color"), 0, 1, 0, 0);


	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(pos));
		glDrawElements(GL_TRIANGLES, sizeof(id), GL_UNSIGNED_INT, nullptr);

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}