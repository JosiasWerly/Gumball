#pragma once
#ifndef _shader
#define _shader
#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "Math.hpp"
#include "Texture.hpp"

#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
using namespace std;

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

struct ShaderUniform {
	unsigned location;
	EUniformType type;
	string name;
	class IShaderUniformBus *bus;

	ShaderUniform(unsigned location, EUniformType type, string name) :
		location(location), type(type), name(name), bus(nullptr) { }
};

class IShaderUniformBus {
public:
	const ShaderUniform &owner;
	IShaderUniformBus(const ShaderUniform &owner) : owner(owner) {}
	virtual void upload() = 0;

	static IShaderUniformBus *TCreateBus(ShaderUniform &Uniform);
};
template<class T> class TShaderUniformBus : public IShaderUniformBus {
public:
	using IShaderUniformBus::IShaderUniformBus;
	virtual void upload() = 0;
};

#define ShaderUniformDelc(CType, Exp)\
template<> class TShaderUniformBus<CType> : public IShaderUniformBus {\
public:\
	using IShaderUniformBus::IShaderUniformBus;\
	typedef CType Type;\
	Type val;\
	void upload() override final { Exp; } \
}
ShaderUniformDelc(int,			glUniform1iv(owner.location, 1, &val));
ShaderUniformDelc(float,		glUniform1fv(owner.location, 1, &val));
ShaderUniformDelc(glm::fvec2,	glUniform2fv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::fvec3,	glUniform3fv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::fvec4,	glUniform4fv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec2,	glUniform2iv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec3,	glUniform3iv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec4,	glUniform4iv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::mat4,	glUniformMatrix4fv(owner.location, 1, GL_FALSE, &val[0][0]));
//ShaderUniformDelc(Image *,		/*glActiveTexture(GL_TEXTURE0); val->bind();*/);

template<> class TShaderUniformBus<Tbo*> : public IShaderUniformBus {
public:
	using IShaderUniformBus::IShaderUniformBus;
	typedef Tbo* Type;
	Type val = nullptr;
	unsigned activeId = 0;
	void upload() override final { 
		if (!val)
			return;
		glActiveTexture(GL_TEXTURE0 + activeId);
		val->bind();
		glUniform1i(owner.location, activeId);
	}
};
#undef ShaderUniformDelc

class ShaderUniformIOBus {
private:

	unordered_map<string, const ShaderUniform*> parameters;
	//list<unordered_map<string, const ShaderUniform *>::iterator *> toUpload;
	
	template<class T> TShaderUniformBus<T> *param(const string &name) { return dynamic_cast<TShaderUniformBus<T>*>(parameters[name]->bus); }
public:
	ShaderUniformIOBus();
	~ShaderUniformIOBus();

	void attach(const list<ShaderUniform> &uniforms);
	void detach();
	void upload() const;
	const ShaderUniform *getUniform(string name) { return parameters[name]; }

	template<class T> void setParam(const string &name, T) = delete;
	template<class T> T getParam(const string &name) = delete;
	#define ParamDelc(CType)\
	template<> void setParam<CType>(const string &name, CType value) { \
		param<CType>(name)->val = value;\
	}\
	template<> CType getParam<CType>(const string &name) { \
		return param<CType>(name)->val;\
	}
	ParamDelc(int);
	ParamDelc(float);
	ParamDelc(glm::mat4);
	ParamDelc(glm::fvec2);
	ParamDelc(glm::fvec3);
	ParamDelc(glm::fvec4);
	ParamDelc(glm::ivec2);
	ParamDelc(glm::ivec3);
	ParamDelc(glm::ivec4);
	ParamDelc(Tbo *);
	#undef ParamDelc

	template<> void setParam<Color>(const string &name, Color value) {
		param<glm::fvec4>(name)->val = Color::toVec4(value);
	}
	template<> Color getParam<Color>(const string &name) {
		return Color::toColor(param<glm::fvec4>(name)->val);
	}

	template<> void setParam<Vector3>(const string &name, Vector3 value) {
		param<glm::fvec3>(name)->val = value.rawVector;
	}
	template<> Vector3 getParam<Vector3>(const string &name) {		
		const auto &v = param<glm::fvec3>(name)->val;
		return Vector3::Vector3(v.x, v.y, v.z);
	}
};

class GBCORE Shader {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

protected:
	unsigned shaderId = 0;
	list<ShaderUniform> uniforms;
	ShaderUniformIOBus uniformIOBus;

	void updateUniforms();
	bool compile(EShaderType eShaderType, const string &code, int &id);
public:
	Shader();
	virtual ~Shader();
	bool create(const string &vertex, const string &fragment);
	Inline void bind() const;
	Inline void unbind() const;
	Inline void upload() const;
	list<ShaderUniform> &getUniforms() { return uniforms; }
	ShaderUniformIOBus &uniformIO() { return uniformIOBus; }
};

class GBCORE ShaderInstance {
protected:
	Shader *shader;
	ShaderUniformIOBus uniformIOBus;
public:
	void setShader(Shader *newShader);
	Shader *getShader() { return shader; }
	ShaderUniformIOBus &uniformIO() { return uniformIOBus; }

	Inline void bind() const { shader->bind(); }
	Inline void unbind() const { shader->unbind(); }
	Inline void upload() const { uniformIOBus.upload(); }
};

template<> class AssetFactory<Shader> : public TAssetFactory<Shader> {
public:
	AssetFactory();
	bool load(Archive &ar, Shader &val);
	bool save(Archive &ar, const Shader &val);
};

#endif // !_shader