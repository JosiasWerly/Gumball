#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shaders.hpp"

using namespace std;
int main() {
	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
		-0.1, -0.1, 0.0,
		0.0, 0.0, 0.1,
		0.1, -0.1, 0.0
	};

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(0);

	

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


	
	if (glGetAttribLocation(sh, "test") != -1)
		glVertexAttrib1f(glGetAttribLocation(sh, "test"), 0.5);

	glUniform4f(glGetUniformLocation(sh, "color"), 0, 1, 0, 0);
	//glUniform1f(glGetUniformLocation(sh, "test"), 0.5);


	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



//int main(void){
//    
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//
//    glfwMakeContextCurrent(window);
//    while (!glfwWindowShouldClose(window)){
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glBegin(GL_TRIANGLES);
//        glColor3f(0.1, 0.2, 0.3);
//        glVertex2f(-0.5, -0.5);
//        glVertex2f(0, 0.5);
//        glVertex2f(0.5, -0.5);
//        glEnd();
//
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}