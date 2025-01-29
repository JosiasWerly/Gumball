#pragma once
#ifndef _shader
#define _shader
#include <Asset/Asset.hpp>
#include "ShaderUniform.hpp"

class GMODULE Shader {
public:
	enum class EShaderType { Vertex, Fragment };

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

class GMODULE ShaderInstance {
protected:
	Shader *shader;
	ShaderParams params;
public:
	void setShader(Shader *newShader);
	Shader *getShader() { return shader; }
	ShaderParams &getParameters() { return params; }
};

template<>
class GMODULE WAssetBuilder<Shader> : public TAssetBuilder<Shader> {
public:
	bool load(Archive &ar, Shader &val);
	bool save(Archive &ar, Shader &val);
	bool hasExtension(const string &extention) const;
};
#endif // !_shader