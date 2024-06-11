#include "ShaderUniform.hpp"
#include "GLUtils.hpp"

#define ShaderUniformDelc(CType, Exp)\
template<> class TShaderUniformBus<CType> : public ShaderUniformBus {\
public:\
	using ShaderUniformBus::ShaderUniformBus;\
	typedef CType Type;\
	Type val;\
	void upload() override final { Exp; } \
}
ShaderUniformDelc(int, glUniform1iv(info.location, 1, &val));
ShaderUniformDelc(float, glUniform1fv(info.location, 1, &val));
ShaderUniformDelc(glm::fvec2, glUniform2fv(info.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::fvec3, glUniform3fv(info.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::fvec4, glUniform4fv(info.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec2, glUniform2iv(info.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec3, glUniform3iv(info.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::ivec4, glUniform4iv(info.location, 1, glm::value_ptr(val)));
ShaderUniformDelc(glm::mat4, glUniformMatrix4fv(info.location, 1, GL_FALSE, &val[0][0]));
#undef ShaderUniformDelc


void TShaderUniformBus<Texture *>::upload() {
	glActiveTexture(GL_TEXTURE0 + activeId);
	val->bind();
	glUniform1i(info.location, activeId);
}

ShaderUniformBus *createBus(const ShaderUniformInfo &UniformInfo) {
	switch (UniformInfo.type) {
		case EUniformType::uint:			return new TShaderUniformBus<int>(UniformInfo);				break;
		case EUniformType::ufloat:			return new TShaderUniformBus<float>(UniformInfo);			break;
		case EUniformType::ufvec2:			return new TShaderUniformBus<glm::fvec2>(UniformInfo);		break;
		case EUniformType::ufvec3:			return new TShaderUniformBus<glm::fvec3>(UniformInfo);		break;
		case EUniformType::ufvec4:			return new TShaderUniformBus<glm::fvec4>(UniformInfo);		break;
		case EUniformType::uivec2:			return new TShaderUniformBus<glm::ivec2>(UniformInfo);		break;
		case EUniformType::uivec3:			return new TShaderUniformBus<glm::ivec3>(UniformInfo);		break;
		case EUniformType::uivec4:			return new TShaderUniformBus<glm::ivec4>(UniformInfo);		break;
		case EUniformType::umat:			return new TShaderUniformBus<glm::mat4>(UniformInfo);		break;
		case EUniformType::utexture:		return new TShaderUniformBus<Texture *>(UniformInfo);		break;
		default: throw;
	}
}

ShaderUniforms::ShaderUniforms() {
}
ShaderUniforms::~ShaderUniforms() {
	clear();
}
void ShaderUniforms::__add(ShaderUniformBus *&uniformBus, const bool &activate) {
	uniforms.push_back(uniformBus);
	accessor[uniformBus->info.name] = uniformBus;
	if (activate) {
		active[uniformBus->info.name] = uniformBus;
	}
}
void ShaderUniforms::__genAccessor(list<ShaderUniformBus *> uniforms) {
	accessor.clear();
	for (auto &u : uniforms) {
		accessor[u->info.name] = u;
	}
}
ShaderUniformInfo &ShaderUniforms::__getInfo(const string &name) {
	for (auto &i : uniformsInfo) {
		if (i.name == name) {
			return i;
		}
	}
	throw;
}
void ShaderUniforms::__activate(string name) {
	active[name] = accessor[name];
}
void ShaderUniforms::__deactivate(string name) {
	active.erase(name);
}
ShaderUniformBus *ShaderUniforms::add(const ShaderUniformInfo &info, bool autoActivate) {
	uniformsInfo.push_back(info);
	ShaderUniformBus *newBus = createBus(uniformsInfo.back());
	__add(newBus, autoActivate);
	return newBus;
}
void ShaderUniforms::del(string name) {
	if (accessor.contains(name)) {
		ShaderUniformBus *bus = accessor[name];
		uniforms.remove(bus);
		accessor.erase(name);
		active.erase(name);
		uniformsInfo.remove(bus->info);
		delete bus;
	}
}
void ShaderUniforms::setActive(string name, bool newActive) {
	if (accessor.contains(name)) {
		if (newActive) {
			__activate(name);
		}
		else {
			__deactivate(name);
		}
	}
}
void ShaderUniforms::activate(list<string> names) {
	for (auto &name : names) {
		if (accessor.contains(name)) {
			__activate(name);
		}
	}
}
void ShaderUniforms::deactivate(list<string> names) {
	for (auto &name : names) {
		__deactivate(name);
	}
}
void ShaderUniforms::clear() {
	active.clear();
	accessor.clear();
	for (ShaderUniformBus *i : uniforms) {
		delete i;
	}
	uniforms.clear();
}
void ShaderUniforms::uploadActive() const {
	for (auto kv : active) {
		kv.second->upload();
	}
}
void ShaderUniforms::uploadAll() const {
	for (auto &i : uniforms) {
		i->upload();
	}
}
#define UniformImpl(CType, GType)\
template<> void ShaderUniforms::sync<CType>(const string &name, CType value) { \
	auto *p = accessor[name]->as<GType>();\
	p->val = value;\
	p->upload();\
}\
template<> void ShaderUniforms::set<CType>(const string &name, CType value) { \
	accessor[name]->as<GType>()->val = value;\
}\
template<> CType ShaderUniforms::get<CType>(const string &name) { \
	return accessor[name]->as<GType>()->val;\
}
UniformImpl(int, int);
UniformImpl(float, float);
UniformImpl(glm::mat4, glm::mat4);
UniformImpl(glm::fvec2, glm::fvec2);
UniformImpl(glm::fvec3, glm::fvec3);
UniformImpl(glm::fvec4, glm::fvec4);
UniformImpl(glm::ivec2, glm::ivec2);
UniformImpl(glm::ivec3, glm::ivec3);
UniformImpl(glm::ivec4, glm::ivec4);
UniformImpl(Color, glm::fvec4);
UniformImpl(Vector3, glm::fvec3);
UniformImpl(Texture*, Texture *);
#undef UniformImpl


void ShaderParams::__activate(string name) {
	overwriten.insert(name);
	add(parent->__getInfo(name), true);
}
void ShaderParams::__deactivate(string name) {
	if (overwriten.contains(name)) {
		del(name);
	}
	accessor[name] = parent->accessor[name];
	active[name] = parent->active[name];
}
void ShaderParams::setParent(ShaderUniforms *newParent) {
	clear();

	overwriten.clear();
	parent = newParent;
	if (newParent) {
		accessor = parent->accessor;
		active = parent->active;
	}
}