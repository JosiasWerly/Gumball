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
#include <unordered_map>
using namespace std;

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

struct ShaderAttributeDescriptor {
	unsigned location;
	EUniformType type;
	string name;
	ShaderAttributeDescriptor(string name, unsigned location, EUniformType type) :
		name(name),
		location(location),
		type(type) {
	}
};

class GBCORE Shader :
	public Object {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};
	
protected:
	list<ShaderAttributeDescriptor> attributeScheme;

	bool compile(EShaderType eShaderType, const string& code, int& id);
	void captureAttributeSchema();
public:
	virtual Var<Object> clone() const override { return Var<Object>(new Shader); }
	virtual bool archiveLoad(Archive &ar) override;
	virtual bool archiveSave(Archive &ar) override;
	
	unsigned shaderId = 0;

	Shader();
	virtual ~Shader();
	bool create(const string &vertex, const string &fragment);

	Inline void bind() const;
	Inline void unbind() const;
	
	Inline const list<ShaderAttributeDescriptor> &getAttributes() const { return attributeScheme; }
	bool hasAttribute(string name) { 
		for (auto &a : attributeScheme) {
			if (a.name == name)
				return true;
		}
		return false;
	}
	const ShaderAttributeDescriptor* getAttribute(string name) {
		for (auto &a : attributeScheme) {
			if (a.name == name)
				return &a;
		}
		return nullptr;
	}
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
public:
	const ShaderAttribute &owner;
	IShaderParameter(const ShaderAttribute &owner) : owner(owner){}
	virtual void upload() = 0;
};

template<class T> 
class TShaderParameter : 
	public IShaderParameter { 
public:
	using IShaderParameter::IShaderParameter;
	virtual void upload() = 0;
};
#define ShaderParamDelc(CType, Exp)\
template<> class TShaderParameter<CType> : public IShaderParameter {\
public:\
	using IShaderParameter::IShaderParameter;\
	typedef CType Type;\
	CType val;\
	void upload() override { Exp; } \
}
ShaderParamDelc(int,			glUniform1iv(owner.location, 1, &val));
ShaderParamDelc(float,			glUniform1fv(owner.location, 1, &val));
ShaderParamDelc(glm::fvec2,		glUniform2fv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::fvec3,		glUniform3fv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::fvec4,		glUniform4fv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::ivec2,		glUniform2iv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::ivec3,		glUniform3iv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::ivec4,		glUniform4iv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::mat4,		glUniformMatrix4fv(owner.location, 1, GL_FALSE, &val[0][0]));
ShaderParamDelc(Image *,		glActiveTexture(GL_TEXTURE0); val->bind(););
#undef ShaderParamDelc


class GBCORE Material {
private:
	Shader *shader;
	unordered_map<string, ShaderAttribute *> parameters;

	void copyParameters();
	void clearParameters();

	template<class T> TShaderParameter<T> *paramAs(const string &name) {
		
		return dynamic_cast<TShaderParameter<T>*>(parameters[name]->param);
	}
public:
	Material(Shader *sh);
	~Material();

	Inline void bind() {
		shader->bind();
	}
	Inline void unbind() {
		shader->unbind();
	}
	Shader *getShader() { 
		return shader; 
	}

	template<class T> void setParam(const string &name, T) = delete;
	template<class T> T getParam(const string &name) = delete;

	template<> void setParam<Vector3>(const string &name, Vector3 value) {
		auto &p = *paramAs<glm::fvec3>(name);
		p.val = value.rawVector;
		p.upload();
	}
	template<> Vector3 getParam <Vector3>(const string &name) {
		const auto &v = paramAs<glm::fvec3>(name)->val;
		return Vector3(v.x, v.y, v.z);
	}

	template<> void setParam<Color>(const string &name, Color value) {
		auto &p = *paramAs<glm::fvec4>(name);
		p.val = Color::toVec4(value);
		p.upload();
	}
	template<> Color getParam <Color>(const string &name) {
		const auto &v = paramAs<glm::fvec4>(name)->val;
		return Color::toColor(v);
	}

	#define TParamDelc(CType)\
	template<> void setParam<CType>(const string &name, CType value) { \
		auto &p = *paramAs<CType>(name);\
		p.val = value;\
		p.upload();\
	}\
	template<> CType getParam<CType>(const string &name) { \
		return paramAs<CType>(name)->val;\
	}

	TParamDelc(float);
	TParamDelc(int);
	TParamDelc(glm::mat4);
	TParamDelc(Image*);
	#undef TParamDelc
};
#endif // !_shaders