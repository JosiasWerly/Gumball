#pragma once
#ifndef _shader
#define _shader
#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "ShaderUniform.hpp"

#include <fstream>
#include <sstream>

class GBCORE Shader {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

protected:
	unsigned shaderId = 0;
	ShaderUniforms uniforms;

	void updateUniforms();
	bool compile(EShaderType eShaderType, const string &code, int &id);
public:
	Shader();
	virtual ~Shader();
	bool create(const string &vertex, const string &fragment);

	Inline void bind() const;
	Inline void unbind() const;
	Inline void upload() const;
	ShaderUniforms &getUniforms() { return uniforms; }
};


#define ParamDelc(CType, GType)\
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
class GBCORE ShaderInstance {
protected:
	Shader *shader;
	ShaderParams params;
public:
	void setShader(Shader *newShader);
	Shader *getShader() { return shader; }
	ShaderParams &getParameters() { return params; }
};
#undef ParamDelc

template<> class AssetFactory<Shader> : public TAssetFactory<Shader> {
public:
	AssetFactory();
	bool load(Archive &ar, Shader &val);
	bool save(Archive &ar, const Shader &val);
};

#endif // !_shader