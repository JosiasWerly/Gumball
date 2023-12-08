#pragma once
#ifndef __shaderuniform
#define __shaderuniform

#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "Math.hpp"
#include "Texture.hpp"

#include <map>
#include <list>
#include <deque>
#include <unordered_map>
using namespace std;

class ShaderUniformBus;

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

struct ShaderUniformInfo {
	unsigned location;
	EUniformType type;
	string name;
};

class ShaderUniformBus {
public:
	static ShaderUniformBus *createBus(const ShaderUniformInfo &UniformInfo);
	template<class T> TShaderUniformBus<T> *as() { dynamic_cast<TShaderUniformBus<T> *>(this); }

	const ShaderUniformInfo &owner;
	ShaderUniformBus(const ShaderUniformInfo &owner) : owner(owner) {}
	virtual void upload() = 0;
};

template<class T> class TShaderUniformBus : public ShaderUniformBus {
public:
	using ShaderUniformBus::ShaderUniformBus;
	virtual void upload() = 0;
};

#define ShaderUniformDelc(CType, Exp)\
template<> class TShaderUniformBus<CType> : public ShaderUniformBus {\
public:\
	using ShaderUniformBus::ShaderUniformBus;\
	typedef CType Type;\
	Type val;\
	void upload() override final { Exp; } \
}
ShaderUniformDelc(int, glUniform1iv(owner.location, 1, &val));
ShaderUniformDelc(float, glUniform1fv(owner.location, 1, &val));
ShaderUniformDelc(glm::fvec2, glUniform2fv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::fvec3, glUniform3fv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::fvec4, glUniform4fv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec2, glUniform2iv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec3, glUniform3iv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec4, glUniform4iv(owner.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::mat4, glUniformMatrix4fv(owner.location, 1, GL_FALSE, &val[0][0]));
//ShaderUniformDelc(Image *,		/*glActiveTexture(GL_TEXTURE0); val->bind();*/);
#undef ShaderUniformDelc
template<> class TShaderUniformBus<Tbo *> : public ShaderUniformBus {
public:
	using ShaderUniformBus::ShaderUniformBus;
	typedef Tbo *Type;
	Type val = nullptr;
	unsigned activeId = 0;
	void upload() override final {
		//if (!val) return;
		glActiveTexture(GL_TEXTURE0 + activeId);
		val->bind();
		glUniform1i(owner.location, activeId);
	}
};

class ShaderUniforms {
private:
	list<ShaderUniformBus *> uniforms;
	unordered_map<string, ShaderUniformBus *> accessor;
	unordered_map<string, ShaderUniformBus *> active;

	Inline void __add(ShaderUniformBus *&uniformBus, const bool &activate) {
		uniforms.push_back(uniformBus);
		accessor[uniformBus->owner.name] = uniformBus;
		if (activate) {
			active[uniformBus->owner.name] = uniformBus;
		}
	}

public:
	ShaderUniforms();
	~ShaderUniforms();

	ShaderUniformBus *clone(ShaderUniformBus *target, bool activate = false);
	ShaderUniformBus *add(const ShaderUniformInfo &info, bool activate = false);
	void del(string name);
	void setActive(string name, bool newActive);
	void clear();

	void uploadActive() const;
	void uploadAll() const;


	template<class T> void sync(const string &name, T) = delete;
	template<class T> void set(const string &name, T) = delete;
	template<class T> T get(const string &name) = delete;	
	
	#define UniformDelc(CType, GType)\
	template<> void sync<CType>(const string &name, CType value) { \
		auto *p = accessor[name]->as<GType>();\
		p->val = value;\
		p->upload();\
	}\
	template<> void set<CType>(const string &name, CType value) { \
		accessor[name]->as<GType>()->val = value;\
	}\
	template<> CType get<CType>(const string &name) { \
		return accessor[name]->as<GType>()->val;\
	}
	UniformDelc(int, int);
	UniformDelc(float, float);
	UniformDelc(glm::mat4, glm::mat4);
	UniformDelc(glm::fvec2, glm::fvec2);
	UniformDelc(glm::fvec3, glm::fvec3);
	UniformDelc(glm::fvec4, glm::fvec4);
	UniformDelc(glm::ivec2, glm::ivec2);
	UniformDelc(glm::ivec3, glm::ivec3);
	UniformDelc(glm::ivec4, glm::ivec4);
	UniformDelc(Tbo *, Tbo *);
	UniformDelc(Color, glm::fvec4);
	UniformDelc(Vector3, glm::fvec3);	
	#undef UniformDelc;
};

class ShaderParams {
private:
	ShaderUniforms *uniformsTarget;
	unordered_map<string, ShaderUniformBus *> accessor;

public:
	void attach(ShaderUniforms *target);
	void detach();

	void setEnable(string name, bool newEnable);


	template<class T> void set(const string &name, T) = delete;
	template<class T> T get(const string &name) = delete;
	
	#define ParamDelc(CType, GType)\
	template<> void set<CType>(const string &name, CType value) { \
		accessor[name]->as<GType>()->val = value;\
	}\
	template<> CType get<CType>(const string &name) { \
		return accessor[name]->as<GType>()->val;\
	}
	ParamDelc(int, int);
	ParamDelc(float, float);
	ParamDelc(glm::mat4, glm::mat4);
	ParamDelc(glm::fvec2, glm::fvec2);
	ParamDelc(glm::fvec3, glm::fvec3);
	ParamDelc(glm::fvec4, glm::fvec4);
	ParamDelc(glm::ivec2, glm::ivec2);
	ParamDelc(glm::ivec3, glm::ivec3);
	ParamDelc(glm::ivec4, glm::ivec4);
	ParamDelc(Tbo *, Tbo *);
	ParamDelc(Color, glm::fvec4);
	ParamDelc(Vector3, glm::fvec3);
	#undef ParamDelc;
};

#endif