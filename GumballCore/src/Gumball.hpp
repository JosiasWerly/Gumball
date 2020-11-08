#ifndef _gumball
#define _gumball

#include "Math.hpp"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Patterns.hpp"



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
        cout << "[openGL " << err << "] " << fnx << " " << file << " " << line;
        return false;
    }
    return true;
}


namespace gbLib {
    string getNameOfFilePath(string path) {
        string fName = std::filesystem::path(path).filename().string();
        return fName.substr(0, fName.find_last_of("."));
    }
};

namespace Gumball {
    class Window {
        int x, y;
        GLFWwindow* window;
    public:
        Window() {

        }
        ~Window() {

        }

        GLFWwindow* getWindow() {
            return window;
        }

        float getAspec() {
            return ((float)x / (float)y);
        }
        void create(string winName, int x, int y) {
            this->x = x;
            this->y = y;
            glfwInit();
            //cout << "glVersion " << glGetString(GL_VERSION) << endl;
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //profile

            window = glfwCreateWindow(x, y, winName.c_str(), NULL, NULL);
            if (!window) {
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            glfwMakeContextCurrent(window);
            
            if (!gladLoadGL())
                cout << "fail to load window" << endl;

            glViewport(0, 0, x, y);
            glEnable(GL_DEPTH_TEST);
            glfwSwapInterval(1);


            ///////////////////Old stuff//////////////////////
            //textureAlpha
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //endTextureAlpha
        }
        void destroy() {
            glfwTerminate();
        }
    };
};
#endif // !_gumball
