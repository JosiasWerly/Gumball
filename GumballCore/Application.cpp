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

	struct Draw {
	private:
		struct Ibo{
			unsigned id, size;
			
			Ibo() {
				glGenBuffers(1, &id);
			}
			~Ibo() {
				glDeleteBuffers(1, &id);
			}
			inline void bind() {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			}
			inline void unbind() {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			inline void setBuffer(void* data, unsigned dataSize) {
				size = dataSize;
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
			}
		};
		struct Vbo{
			unsigned id;
			Vbo() {
				glGenBuffers(1, &id);
			}
			~Vbo() {
				glDeleteBuffers(1, &id);
			}
			inline void bind() {
				glBindBuffer(GL_ARRAY_BUFFER, id);
			}
			inline void unbind() {
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			inline void setBuffer(void* data, unsigned dataSize) {				
				glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
			}
			inline void attribLayout(unsigned attribID, unsigned size, unsigned stride, unsigned type = GL_FLOAT) {
				glVertexAttribPointer(attribID, size, type, GL_FALSE, stride, 0);
				glEnableVertexAttribArray(attribID);
			}
		};
		struct Vao{
		private:
			unsigned vao;
		public:
			Vao() {
				glGenVertexArrays(1, &vao);				
			}
			~Vao() {
				glDeleteVertexArrays(1, &vao);
			}
			inline void bind() {
				glBindVertexArray(vao);
			}
			inline void unBind() {
				glBindVertexArray(0);
			}
		};
	public:
		Vao vao;
		Vbo vbo;
		Ibo ibo;
		Draw() {
			vao.bind();
			vbo.bind();
			vbo.setBuffer(
				new float[12]{
					0, 0, 0,
					0.1, 0.0, 0,
					0.1, 0.1, 0.0,
					0, 0.1, 0.0
				},
				12 * sizeof(float));
			vbo.attribLayout(0, 3, 3 * sizeof(float));
			

			ibo.bind();
			ibo.setBuffer(
				new unsigned[5] {
					0, 1, 2,
					2, 3
				},
				5 * sizeof(float));
			vao.unBind();
			vbo.unbind();
			ibo.unbind();
		}

		inline void draw() {
			vao.bind();
			glDrawElements(GL_TRIANGLES, ibo.size, GL_UNSIGNED_INT, nullptr);
		}
	};

	//unsigned vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//
	//
	//
	//unsigned vbo;
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	//
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glEnableVertexAttribArray(0);
	//
	//
	//unsigned ibo;
	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(id), id, GL_STATIC_DRAW);
	Draw d;

	auto sh = shaders["default"];
	glUseProgram(sh);


	
	glVertexAttrib1f(glGetAttribLocation(sh, "test"), 0.5);
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