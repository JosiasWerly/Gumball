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
class Material;


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
	friend class Material;
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
  
struct ShaderAttributeDescriptor {
	string name;
	unsigned location;
	EUniformType type;
	ShaderAttributeDescriptor(string name, unsigned location, EUniformType type) :
		name(name),
		location(location),
		type(type) {
	}
};
class Shader :
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
	unsigned shaderId = 0;

	Shader();
	virtual ~Shader();
	bool create(const string &vertex, const string &fragment);

	Inline void bind();
	Inline void unBind() const;
	
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


	virtual Var<Object> clone() const override { return Var<Object>(new Shader); }
	virtual bool archiveLoad(Archive &ar) override;
	virtual bool archiveSave(Archive &ar) override;
};

//TODO: this class should be moved to SceneOverlay.hpp, because the close relation between both
class GBCORE Material {
private:
	Shader *shader;
	map<string, ShaderAttribute *> parameters;

	Inline void copyParameters();
	Inline void clearParameters();
public:
	Material();
	~Material();
	void setShader(Shader *shader);
	const Shader *getShader() { return shader; }
	bool hasParameter(string name) const { return parameters.contains(name); }
	template<EUniformType etype> ShaderParameter<etype> *param(string name) {
		return dynamic_cast<ShaderParameter<etype>*>(parameters[name]->param);
	}

	Inline void bind();
	Inline void unBind();
	Inline void uploadParams();
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