

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <set>
#include <vector>
#include <array>
using namespace std;

#define glAssert(fnx) if (!(fnx)) __debugbreak();
#define glDCall(fnx)\
GLClearError();\
fnx;\
glAssert(GLLogCall(#fnx, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* fnx, const char* file, int line) {
    while (GLenum err = glGetError()) {
        cout << "[openGL error] : " << fnx << " " << file << " " << line;
        return false;
    }
    return true;
}






class iDrawCall {
public:
    virtual void draw() = 0;
};

class Window {
public:
    int x, y;
    GLFWwindow* window;
    set<iDrawCall*> drawcalls;

    Window() {
    }
    ~Window() {
        glfwTerminate();
    }
    void setup(string winName, int x, int y) {
        this->x = x;
        this->y = y;

        glfwInit();
        //cout << "glVersion " << glGetString(GL_VERSION) << endl;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profile
        gladLoadGL();
        window = glfwCreateWindow(x, y, winName.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        gladLoadGL();
        glViewport(0, 0, x, y);
        glfwSwapInterval(1);
    }
    void draw() {
        for (auto& d : drawcalls)
            d->draw();
    }
    void clear() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    void swap() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
};

class debugDraw : public iDrawCall {
private:
    unsigned int vbo, vba, vea, shader;
public:
    debugDraw() {
        glDCall(glGenVertexArrays(1, &vba));
        glDCall(glGenBuffers(1, &vbo));
        glDCall(glGenBuffers(1, &vea));
    }
    ~debugDraw() {
        glDeleteVertexArrays(1, &vba);
        glDeleteBuffers(1, &vea);
        glDeleteBuffers(1, &vbo);
        glDeleteProgram(shader);
    }
    void setup(unsigned int shader, vector<float> data, vector<unsigned int> index) {
        this->shader = shader;
        glDCall(glBindVertexArray(vba));
        glDCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        glDCall(glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW));
        
        int i = data.size() * sizeof(float);

        
        glDCall(glEnableVertexAttribArray(0)); //???
        glDCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr));
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vea));
        glDCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data(), GL_STATIC_DRAW));
    }
    void draw() {
        glDCall(glUseProgram(shader));
        glDCall(glUniform4f(glGetUniformLocation(shader, "uColor"), 1, 0, 0, 1));
        glDCall(glBindVertexArray(vba));
        glDCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vea));
        glDCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }
};