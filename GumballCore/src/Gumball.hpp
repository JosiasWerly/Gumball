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

#endif // !_gumball
