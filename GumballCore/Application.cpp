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
	//string vs = 
	//	"#version 330 core\n"
	//	"layout (location = 0) in vec4 pos;\n"
	//	"out vec4 vColor;\n"
	//	"void main(){\n"
	//	"vColor = vec4(sin(pos.x), cos(pos.y), 0.0, 1.0);\n"
	//	"gl_Position = pos;\n"
	//	"}\n";
	//string vf =
	//	"#version 330 core\n"
	//	"uniform float time;\n"
	//	"in vec4 vColor;\n"		
	//	"void main(){\n"
	//	"gl_FragColor = vec4(sin(gl_FragCoord.x), cos(gl_FragCoord.y), 0, 1.0);\n"
	//	"}\n";
	//unsigned sh = createShader(vs, vf);
	unsigned sh = shaders["default"];
	unsigned int i;
	float pos[6] = {
		-0.5, -0.5,
		0.0, 0.5,
		0.5, -0.5
	};
	glGenBuffers(1, &i);
	glBindBuffer(GL_ARRAY_BUFFER, i);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, pos, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glUseProgram(sh);


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