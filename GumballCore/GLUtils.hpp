#pragma once
#ifndef _glutils
#define _glutils

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define glAssert(fnx) if (!(fnx)) __debugbreak();
#define glDCall(fnx)\
GLClearError();\
fnx;\
glAssert(GLLogCall(#fnx, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char *fnx, const char *file, int line) {
	while (GLenum err = glGetError()) {
		std::cout << "[openGL " << err << "] " << fnx << " " << file << " " << line;
		return false;
	}
	return true;
}


namespace Conversors {
	template<class T> constexpr unsigned typeCToGL() = delete;
	#define pushType(CType, GLType)template<> constexpr unsigned typeCToGL<CType>() { return GLType; }
	pushType(char *, GL_BYTE)
	pushType(bool, GL_BOOL)
	pushType(int, GL_INT)
	pushType(float, GL_FLOAT)
	#undef pushType


	template<int> constexpr unsigned sizeGLToC() = delete;
	#define pushType(GLType, CSize)template<> constexpr unsigned sizeGLToC<GLType>() { return CSize; }
	pushType(GL_FLOAT, 4)
	pushType(GL_INT, 4)
	pushType(GL_BYTE, 1)
	#undef pushType
};

#endif // !_glutils