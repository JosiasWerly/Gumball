#ifndef _gFunctionLibrary
#define _gFunctionLibrary

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>


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

string getExtOfFilePath(string path);
string getNameOfFilePath(string path);

#endif // !_gFunctionLibrary
