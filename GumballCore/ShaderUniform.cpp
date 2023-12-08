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
ShaderUniformBus *ShaderUniforms::clone(ShaderUniformBus *target, bool activate) {
	if (!target) {
		return nullptr;
	}
	ShaderUniformBus *clonedBus = ShaderUniformBus::createBus(target->owner);
	__add(clonedBus, activate);
}
ShaderUniformBus *ShaderUniforms::add(const ShaderUniformInfo &info, bool activate) {
	ShaderUniformBus *newBus = ShaderUniformBus::createBus(info);
	__add(newBus, activate);	
}
void ShaderUniforms::del(string name) {
	if (accessor.contains(name)) {
		ShaderUniformBus *bus = accessor[name];
		uniforms.remove(bus);
		accessor.erase(name);
		active.erase(name);
		delete bus;
	}
}
void ShaderUniforms::setActive(string name, bool newActive) {
	if (accessor.contains(name)) {
		if (newActive) {
			active[name] = accessor[name];
		}
		else {
			active.erase(name);
		}
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