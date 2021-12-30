#pragma once
#ifndef __shaders
#define __shaders
#include "AssetManager.hpp"
#include "GLUtils.hpp"

#include <fstream>
#include <sstream>
#include <map>
using namespace std;
class Shader;

class ShaderFactory : 
	public IAssetFactory {
public:
	ShaderFactory() : 
		IAssetFactory("ShaderFactory") {
		this->extensions = { "shader", "glsl" };
	}
	virtual bool assemble(Asset &asset, Archive &ar);
	virtual bool disassemble(Asset &asset, Archive &ar) {
		return true;
	}

	Inline void makeSourceFromArchive(Archive& ar, string &vertexCode, string &fragmentCode) {
		string outString[2];
		enum ESType{
			none = -1, vertex, fragment
		} shType;
		string line;
		while (ar.getLine(line)) {
			if (line.find("#vert") != string::npos)
				shType = ESType::vertex;
			else if (line.find("#frag") != string::npos)
				shType = ESType::fragment;
			else if (line.find("//") == string::npos)
				outString[static_cast<int>(shType)] += line + "\n";
		}
		vertexCode = outString[ESType::vertex];
		fragmentCode = outString[ESType::fragment];
	}
};





class ShaderParamData {
public:
	const unsigned location;
	const unsigned type;
};
class ShaderParamFunctionoid {
public:
	virtual void upload() = 0;
};
class ShaderParam {
	virtual void upload() = 0;
};
class ShaderParameters{
private:
	ShaderParamBind* newShaderBind(unsigned type);
	Inline void clearUniform();
public:
	const Shader& owner;
	map<string, ShaderParam> uniforms;
	ShaderParameters(const Shader& owner) :
		owner(owner) {

	}
	Inline void updateUniforms();

};


class Shader {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	unsigned shaderId = 0;
	ShaderParameters parameters;
	
	Shader() : 
		parameters(*this) {}
	bool create(const string &vertex, const string &fragment);
	bool compile(EShaderType eShaderType, const string& code, int& id);

	inline void bind(){
		glUseProgram(shaderId);
	}
	inline void unBind() {
		glUseProgram(0);
	}
};

class Material {
	Shader* shader = nullptr;
public:

	bool setShader(string name) {
		Shader* sh = nullptr;
		if (AssetsSystem::instance()(name, sh))
			shader = sh;
		return shader;
	}
	bool isInstance() { return false; }
};

#endif // !__shaders

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