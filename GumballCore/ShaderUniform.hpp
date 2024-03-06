#pragma once
#ifndef __shaderuniform
#define __shaderuniform

#include "AssetSystem.hpp" //TODO: remove?
#include "GLUtils.hpp"
#include "Math.hpp"
#include "Texture.hpp"

#include <map>
#include <list>
#include <deque>
#include <set>
#include <unordered_map>
using namespace std;

class ShaderUniformBus;
template<class T> class TShaderUniformBus;

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

	bool operator==(const ShaderUniformInfo &other) const {
		return location == other.location && name == other.name;
	}
};

class ShaderUniformBus {
public:
	static ShaderUniformBus *createBus(const ShaderUniformInfo &UniformInfo);
	template<class T> TShaderUniformBus<T> *as() { return dynamic_cast<TShaderUniformBus<T> *>(this); }

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
	friend class ShaderParams;

protected:
	list<ShaderUniformInfo> uniformsInfo;
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
	Inline void __genAccessor(list<ShaderUniformBus *> uniforms) {
		accessor.clear();
		for (auto &u : uniforms) {
			accessor[u->owner.name] = u;
		}
	}
	Inline ShaderUniformInfo &__getInfo(const string &name) {
		for (auto &i : uniformsInfo) {
			if (i.name == name) {
				return i;
			}
		}
		throw;
	}

	virtual void __activate(string name) {
		active[name] = accessor[name];
	}
	virtual void __deactivate(string name) {
		active.erase(name);
	}

public:
	ShaderUniforms();
	~ShaderUniforms();	

	ShaderUniformBus *add(const ShaderUniformInfo &info, bool autoActivate = false);
	void del(string name);
	void clear();
	void uploadActive() const;
	void uploadAll() const;	

	virtual void setActive(string name, bool newActive);
	virtual void activate(list<string> names);
	virtual void deactivate(list<string> names);

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

#undef UniformDelc

	list<ShaderUniformBus *> &getUniforms() { return uniforms; }
	unordered_map<string, ShaderUniformBus *> &getActive() { return active; }
};

class ShaderParams : private ShaderUniforms {
private:
	ShaderUniforms *parent;
	std::set<string> overwriten;

	void __activate(string name) override;
	void __deactivate(string name) override;
public:
	using ShaderUniforms::set;
	using ShaderUniforms::get;
	using ShaderUniforms::uploadActive;

	using ShaderUniforms::setActive;
	using ShaderUniforms::deactivate;
	using ShaderUniforms::activate;

	void setParent(ShaderUniforms *newParent);
};

#endif