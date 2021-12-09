#pragma once
#ifndef __shaders
#define __shaders
#include "AssetManager.hpp"
#include "GLUtils.hpp"

#include <fstream>
#include <sstream>
#include <map>
using namespace std;


class ShaderFunctionsLibrary {
	ShaderFunctionsLibrary(const ShaderFunctionsLibrary &) = delete;
public:
	struct ShaderSource {
		string vertex, fragment;
	};
	static ShaderSource loadShaderCodeFromFile(string filePath) {
		ifstream fileStream(filePath);
		string line;
		ShaderSource out;
		string outString[2];
		enum eShaderType {
			none = -1, vertex, fragment
		} eShType;
		while (getline(fileStream, line)) {
			if (line.find("#vert") != string::npos)
				eShType = eShaderType::vertex;
			else if (line.find("#frag") != string::npos)
				eShType = eShaderType::fragment;
			else if (line.find("//") == string::npos)
				outString[eShType] += line + "\n";
		}
		return { outString[eShaderType::vertex], outString[eShaderType::fragment] };
	}
	static int compileShader(unsigned int ShaderType, const string &code) {
		unsigned int id = glCreateShader(ShaderType);
		const char *src = code.c_str();
		glDCall(glShaderSource(id, 1, &src, 0));
		glDCall(glCompileShader(id));

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char *msg = (char *)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, msg);

			cout << (ShaderType == GL_VERTEX_SHADER ? "vert" : "frag") << msg << endl;
			return 0;
		}
		return id;
	}
	static int buildShader(const string &vertexShader, const string &fragmentShader) {
		unsigned int
			p = glCreateProgram(),
			vs = compileShader(GL_VERTEX_SHADER, vertexShader),
			fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(p, vs);
		glAttachShader(p, fs);
		glLinkProgram(p);
		glValidateProgram(p);

		int valid;
		glGetProgramiv(p, GL_LINK_STATUS, &valid);
		if (!valid)
			cout << "program shader error" << endl;
		glDeleteShader(vs);
		glDeleteShader(fs);
		return p;
	}
	static int buildShaderFile(string filePath) {
		auto shaderCode = loadShaderCodeFromFile(filePath);
		return buildShader(shaderCode.vertex, shaderCode.fragment);
	}
	static map<string, int *> getActiveUniforms(const unsigned int shaderProgram) {
		map<string, int *> out;
		return out;
	}
	/*static iParamStorage *reflectGLEnum(GLenum type) {
		iParamStorage *out = 0;
		switch (type) {
		case GL_FLOAT:          out = new Uniform<float>;       break;
		case GL_INT:            out = new Uniform<int>;         break;
		case GL_SAMPLER_2D:     out = new Uniform<int>;         break;


		case GL_FLOAT_VEC2:     out = new Uniform<glm::fvec2>;      break;
		case GL_FLOAT_VEC3:     out = new Uniform<glm::fvec3>;      break;
		case GL_FLOAT_VEC4:     out = new Uniform<glm::fvec4>;      break;
		case GL_INT_VEC4:       out = new Uniform<glm::ivec4>;      break;
		case GL_INT_VEC3:       out = new Uniform<glm::ivec3>;      break;
		case GL_INT_VEC2:       out = new Uniform<glm::ivec2>;      break;
		case GL_FLOAT_MAT4:     out = new Uniform<glm::mat4>;       break;
		default:
			cout << "reflectGLEnumError" << endl;
			throw;
		}
		return out;
	}*/
};




class ShaderFactory : public IAssetFactory {
public:
	ShaderFactory() : 
		IAssetFactory("ShaderFactory") {
		this->extensions = { "shader", "glsl" };
	}
	virtual bool load(const string &fileExt, Asset *&Asset) {
		
	}
	virtual bool unload(const string &fileExt, Asset *&Asset) {
	}
};
#endif // !__shaders
//static void loadShaders(string path) {
//    namespace fs = std::filesystem;
//    for (fs::recursive_directory_iterator i(path), end; i != end; ++i) {
//        if (!is_directory(i->path())) {
//            if (i->path().extension().string() == ".shader") {
//                cout << i->path().filename().string() << endl;
//                ifstream fileStream(i->path());
//                string outString[2];
//                string line;
//
//                string shaderSrc[2];
//                enum eShaderType { none = -1, vertex, fragment } eShaderType;
//                while (getline(fileStream, line)) {
//                    if (line.find("#vert") != string::npos)
//                        eShaderType = eShaderType::vertex;
//                    else if (line.find("#frag") != string::npos)
//                        eShaderType = eShaderType::fragment;
//                    else if (line.find("//") == string::npos)
//                        shaderSrc[eShaderType] += line + "\n";
//                }
//                if (unsigned shaderId = buildShader(shaderSrc[eShaderType::vertex], shaderSrc[eShaderType::fragment]))
//                    shaders.emplace(i->path().filename().string().substr(0, i->path().filename().string().find_last_of(".")), shaderId);
//            }
//        }
//    }
//}




//#shader vertex
//#version 330 core
//in vec4 pos;
//in vec2 texCoord;
//out vec2 vTexture;
//
////uniform mat4 uProj, uView, uModel;
//uniform mat4 uProj;
//uniform mat4 uView;
//uniform mat4 uModel;
//void main() {
//	vTexture = texCoord;
//	gl_Position = uProj * uView * uModel * pos;
//};
//
//
//#shader fragment
//#version 330 core
//
//in vec2 vTexture;
//out vec4 color;
//
//uniform vec4 uColor;
//uniform sampler2D uTexture;
//
//void main() {
//	vec4 texColor = texture(uTexture, vTexture);
//	color = texColor * uColor;
//};