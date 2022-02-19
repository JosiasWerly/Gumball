#pragma once
#ifndef __shaders
#define __shaders
#include "AssetManager.hpp"
#include "GLUtils.hpp"

#include <fstream>
#include <sstream>
#include <map>
using namespace std;

class ShaderParam;
class IShaderParamIO;
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


class IShaderParamIO {
public:
	const ShaderParam& owner;
	IShaderParamIO(const ShaderParam& owner) : 
		owner(owner) {
	};
	virtual void upload() = 0;
	virtual void* getData() = 0;
};
class ShaderParam {	
public:
	const unsigned location;
	const unsigned type;
	IShaderParamIO* paramIO;

	ShaderParam(unsigned location, unsigned type);
	~ShaderParam();
};
class ShaderParameters{
public:
	const Shader& owner;
	map<string, ShaderParam*> uniforms;

	ShaderParameters(const Shader& owner) :
		owner(owner) {}
	~ShaderParameters() {
		clearUniforms();
	}
	void clearUniforms();
	void captureUniforms();
	void uploadUniforms();

	template<class T> 
	T* getParamValue(string name) {
		if (uniforms.contains(name)) {
			return reinterpret_cast<T*>(uniforms[name]->paramIO->getData());
		}
		return nullptr;
	}
};

class Shader {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	unsigned shaderId = 0;
	ShaderParameters parameters;
	
	Shader();
	bool create(const string &vertex, const string &fragment);
	bool compile(EShaderType eShaderType, const string& code, int& id);

	Inline void bind();
	Inline void unBind() const;
	Inline void uploadUniforms();
};

class Material {
	Shader* shader = nullptr;
public:
	template<class T> 
	void setParameter(string name, T value);
	void use();
	bool setShader(string name);
	bool isInstance();
};

template<class T>
void Material::setParameter(string name, T value) {
	if (shader) {
		shader->parameters.getParamValue<T>(name);
		if (T *param = shader->parameters.getParamValue<T>(name))
			*param = value;
	}
}


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