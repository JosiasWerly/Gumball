#include "ShaderUniform.hpp"
ShaderUniformBus *ShaderUniformBus::createBus(const ShaderUniformInfo &UniformInfo) {
	switch (UniformInfo.type) {
		case EUniformType::u_int:			return new TShaderUniformBus<int>(UniformInfo);				break;
		case EUniformType::u_float:			return new TShaderUniformBus<float>(UniformInfo);			break;
		case EUniformType::u_fvec2:			return new TShaderUniformBus<glm::fvec2>(UniformInfo);		break;
		case EUniformType::u_fvec3:			return new TShaderUniformBus<glm::fvec3>(UniformInfo);		break;
		case EUniformType::u_fvec4:			return new TShaderUniformBus<glm::fvec4>(UniformInfo);		break;
		case EUniformType::u_ivec2:			return new TShaderUniformBus<glm::ivec2>(UniformInfo);		break;
		case EUniformType::u_ivec3:			return new TShaderUniformBus<glm::ivec3>(UniformInfo);		break;
		case EUniformType::u_ivec4:			return new TShaderUniformBus<glm::ivec4>(UniformInfo);		break;
		case EUniformType::u_mat:			return new TShaderUniformBus<glm::mat4>(UniformInfo);		break;
		case EUniformType::u_stexture:		return new TShaderUniformBus<Tbo *>(UniformInfo);			break;
		default: throw;
	}
}

ShaderUniforms::ShaderUniforms() {
}
ShaderUniforms::~ShaderUniforms() {
	clear();
}
ShaderUniformBus *ShaderUniforms::add(const ShaderUniformInfo &info, bool autoActivate) {
	uniformsInfo.push_back(info);
	ShaderUniformBus *newBus = ShaderUniformBus::createBus(uniformsInfo.back());
	__add(newBus, autoActivate);
	return newBus;
}
void ShaderUniforms::del(string name) {
	if (accessor.contains(name)) {
		ShaderUniformBus *bus = accessor[name];
		uniforms.remove(bus);
		accessor.erase(name);
		active.erase(name);
		uniformsInfo.remove(bus->owner);
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
void ShaderUniforms::clear(){
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
