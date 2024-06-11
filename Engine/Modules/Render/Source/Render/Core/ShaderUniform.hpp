#pragma once
#ifndef __shaderuniform
#define __shaderuniform

#include "Texture.hpp"
#include <string>
#include <unordered_map>
#include <set>

template<class T> class TShaderUniformBus;

enum class EUniformType {
	uint = 0x1404,
	uivec2 = 0x8B53,
	uivec3 = 0x8B54,
	uivec4 = 0x8B55,
	ufloat = 0x1406,
	ufvec2 = 0x8B50,
	ufvec3 = 0x8B51,
	ufvec4 = 0x8B52,
	umat = 0x8B5C,
	utexture = 0x8B5E,
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
	const ShaderUniformInfo &info;

	ShaderUniformBus(const ShaderUniformInfo &info) : info(info) {}
	virtual void upload() = 0;

	template<class T> TShaderUniformBus<T> *as() { return dynamic_cast<TShaderUniformBus<T> *>(this); }
};

template<class T> class TShaderUniformBus : public ShaderUniformBus {
public:
	using ShaderUniformBus::ShaderUniformBus;
	virtual void upload() = 0;
};

template<> class TShaderUniformBus<Texture *> : public ShaderUniformBus {
public:
	using ShaderUniformBus::ShaderUniformBus;
	typedef Texture *Type;
	Type val = nullptr;
	unsigned activeId = 0;

	void upload() override final;
};

class GMODULE ShaderUniforms {
	friend class ShaderParams;

protected:
	list<ShaderUniformInfo> uniformsInfo;
	list<ShaderUniformBus *> uniforms;
	unordered_map<string, ShaderUniformBus *> accessor;
	unordered_map<string, ShaderUniformBus *> active;

	void __add(ShaderUniformBus *&uniformBus, const bool &activate);
	void __genAccessor(list<ShaderUniformBus *> uniforms);
	ShaderUniformInfo &__getInfo(const string &name);
	virtual void __activate(string name);
	virtual void __deactivate(string name);

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
	#define UniformDelc(TType)\
	template<> void sync<TType>(const string &name, TType value); \
	template<> void set<TType>(const string &name, TType value); \
	template<> TType get<TType>(const string &name)	
	UniformDelc(int);
	UniformDelc(float);
	UniformDelc(glm::mat4);
	UniformDelc(glm::fvec2);
	UniformDelc(glm::fvec3);
	UniformDelc(glm::fvec4);
	UniformDelc(glm::ivec2);
	UniformDelc(glm::ivec3);
	UniformDelc(glm::ivec4);
	UniformDelc(Color);
	UniformDelc(Vector3);
	UniformDelc(Texture*);
	#undef UniformDelc

	list<ShaderUniformBus *> &getUniforms() { return uniforms; }
	unordered_map<string, ShaderUniformBus *> &getActive() { return active; }
};

class GMODULE ShaderParams : private ShaderUniforms {
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