#pragma once
#ifndef _shaders
#define _shaders
#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "Math.hpp"
#include "Texture.hpp"

#include <fstream>
#include <sstream>
#include <map>
using namespace std;

class ShaderFactory;
class Shader;
class ShaderAttribute;
class IShaderParameter;

enum class EUniformType {
	u_int = GL_INT,
	u_float = GL_FLOAT,
	u_mat = GL_FLOAT_MAT4,
	u_stexture = GL_SAMPLER_2D,
	u_fvec2 = GL_FLOAT_VEC2,
	u_fvec3 = GL_FLOAT_VEC3,
	u_fvec4 = GL_FLOAT_VEC4,
	u_ivec2 = GL_INT_VEC2,
	u_ivec3 = GL_INT_VEC3,
	u_ivec4 = GL_INT_VEC4,
};

class ShaderAttribute {
public:
	const unsigned location;
	const EUniformType type;
	IShaderParameter *param;

	ShaderAttribute(unsigned location, EUniformType type);
	virtual ~ShaderAttribute();
};
class IShaderParameter {
	friend class Shader;
protected:
	const ShaderAttribute &owner;
	virtual void upload() = 0;
public:
	IShaderParameter(const ShaderAttribute &owner) : 
		owner(owner) {
	}
};
template<EUniformType Type> class ShaderParameter :
	public IShaderParameter {
protected:
	using IShaderParameter::upload;
public:
	using IShaderParameter::IShaderParameter;
};



class ShaderParamSchema {
public:

};
class Shader : 
	public Object {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};
protected:
	bool compile(EShaderType eShaderType, const string& code, int& id);
	map<string, ShaderAttribute*> uniforms;
public:
	unsigned shaderId = 0;

	Shader();
	virtual ~Shader();

	bool create(const string &vertex, const string &fragment);
	void clearUniforms();
	void captureUniforms();
	bool hasUniform(string name);
	template<EUniformType etype> ShaderParameter<etype> *getUniform(string name) {
		return dynamic_cast<ShaderParameter<etype>*>(uniforms[name]->param);
	}


	Inline void bind();
	Inline void unBind() const;
	Inline void uploadUniforms();
};
class Material {
public:
	Var<Shader> shader;
	void use();
};
class ShaderFactory : 
	public IAssetFactory {
public:
	ShaderFactory() : 
		IAssetFactory("ShaderFactory") {
		this->extensions = { "shader", "glsl" };
	}
	virtual bool assemble(Object *&content, Archive &ar);
	virtual bool disassemble(Object *&content, Archive &ar);
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



#define ShaderParamDelc(Type, ValueType, Expression) \
template<> class ShaderParameter<Type> : \
	public IShaderParameter {\
protected:\
	void upload() override { Expression; }\
public:\
	ValueType value = ValueType();\
	using IShaderParameter::IShaderParameter;\
};

ShaderParamDelc(EUniformType::u_int, int, glUniform1iv(owner.location, 1, &value))
ShaderParamDelc(EUniformType::u_float, float, glUniform1fv(owner.location, 1, &value));
ShaderParamDelc(EUniformType::u_mat, glm::mat4, glUniformMatrix4fv(owner.location, 1, GL_FALSE, &value[0][0]));
ShaderParamDelc(EUniformType::u_fvec2, glm::fvec2, glUniform2fv(owner.location, 1, glm::value_ptr(value)));
ShaderParamDelc(EUniformType::u_fvec3, glm::fvec3, glUniform3fv(owner.location, 1, glm::value_ptr(value)));
ShaderParamDelc(EUniformType::u_fvec4, glm::fvec4, glUniform4fv(owner.location, 1, glm::value_ptr(value)));
ShaderParamDelc(EUniformType::u_ivec2, glm::ivec2, glUniform2iv(owner.location, 1, glm::value_ptr(value)));
ShaderParamDelc(EUniformType::u_ivec3, glm::ivec3, glUniform3iv(owner.location, 1, glm::value_ptr(value)));
ShaderParamDelc(EUniformType::u_ivec4, glm::ivec4, glUniform4iv(owner.location, 1, glm::value_ptr(value)));

template<> class ShaderParameter<EUniformType::u_stexture> : 
	public IShaderParameter {
protected:
	void upload() override {
		if (!image)
			return;
		glActiveTexture(GL_TEXTURE0 + slot);
		image->bind();
	}
public:
	using IShaderParameter::IShaderParameter;
	unsigned slot = 0;
	Image *image;
};
#undef ShaderParamDelc

#endif // !_shaders