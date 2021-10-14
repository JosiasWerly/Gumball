#pragma once
#ifndef __shaders
#define __shaders
#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

static map<string, unsigned> shaders;

static unsigned compileShader(unsigned int type, const string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		cout << "#Error "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< message <<
			endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}
static unsigned createShader(const string& vertex, const string& fragment) {
	auto
		vs = compileShader(GL_VERTEX_SHADER, vertex),
		vf = compileShader(GL_FRAGMENT_SHADER, fragment);
	unsigned sProg = glCreateProgram();
	glAttachShader(sProg, vs);
	glAttachShader(sProg, vf);
	glLinkProgram(sProg);
	glValidateProgram(sProg);

	glDeleteShader(vs);
	glDeleteShader(vf);

	

	return sProg;
}


#include <filesystem>
#include <fstream>
#include <sstream>
static void loadShaders(string path) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(path), end; i != end; ++i) {
		if (!is_directory(i->path())) {
			if (i->path().extension().string() == ".shader") {
				cout << i->path().filename().string() << endl;
				ifstream fileStream(i->path());
				string outString[2];
				string line;

				string shaderSrc[2];
				enum eShaderType{ none = -1, vertex, fragment} eShaderType;
				while (getline(fileStream, line)) {
					if (line.find("#vert") != string::npos)
						eShaderType = eShaderType::vertex;
					else if (line.find("#frag") != string::npos)
						eShaderType = eShaderType::fragment;
					else if (line.find("//") == string::npos)
						shaderSrc[eShaderType] += line + "\n";
				}
				if (unsigned shaderId = createShader(shaderSrc[eShaderType::vertex], shaderSrc[eShaderType::fragment]))
					shaders.emplace(i->path().filename().string().substr(0, i->path().filename().string().find_last_of(".")), shaderId);
			}
		}
	}
}
#endif // !__shaders
