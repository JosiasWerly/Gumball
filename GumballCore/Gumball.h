

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
class Window{
public:
	GLFWwindow* window;
    int x, y;
    
    Window() {
    }
	~Window(){
        glfwTerminate();
	}
	void setup(string winName, int x, int y) {
        this->x = x;
        this->y = y;

        glfwInit();
        //cout << "glVersion " << glGetString(GL_VERSION) << endl;
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        gladLoadGL();
        window = glfwCreateWindow(x, y, winName.c_str(), NULL, NULL);
        if (!window){
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        gladLoadGL();
        glViewport(0, 0, x, y);
        glfwSwapInterval(1);
	}
    void clear() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
};