#include "Shaders.hpp"
#include "Engine.hpp"
#include "Math.hpp"
#include "Texture.hpp"



ShaderParam::ShaderParam(unsigned location, EUniformType type) :
	location(location),
	type(type) {

	switch (type) {
	case EUniformType::u_int:			param = new TShaderUniform<EUniformType::u_int>(*this);			break;
	case EUniformType::u_float:			param = new TShaderUniform<EUniformType::u_float>(*this);		break;
	case EUniformType::u_mat:			param = new TShaderUniform<EUniformType::u_mat>(*this);			break;
	case EUniformType::u_stexture:		param = new TShaderUniform<EUniformType::u_stexture>(*this);	break;
	case EUniformType::u_fvec2:			param = new TShaderUniform<EUniformType::u_fvec2>(*this);		break;
	case EUniformType::u_fvec3:			param = new TShaderUniform<EUniformType::u_fvec3>(*this);		break;
	case EUniformType::u_fvec4:			param = new TShaderUniform<EUniformType::u_fvec4>(*this);		break;
	case EUniformType::u_ivec2:			param = new TShaderUniform<EUniformType::u_ivec2>(*this);		break;
	case EUniformType::u_ivec3:			param = new TShaderUniform<EUniformType::u_ivec3>(*this);		break;
	case EUniformType::u_ivec4:			param = new TShaderUniform<EUniformType::u_ivec4>(*this);		break;
	default: throw;
	}
}
ShaderParam::~ShaderParam() {
}


Shader::Shader() {
}
Shader::~Shader() {
	clearUniforms();
}
bool Shader::create(const string &vertex, const string &fragment) {
	shaderId = glCreateProgram();
	int	vs, fs;

	if (!compile(EShaderType::Vertex, vertex, vs))
		return false;
	if (!compile(EShaderType::Fragment, fragment, fs))
		return false;

	glAttachShader(shaderId, vs);
	glAttachShader(shaderId, fs);
	glLinkProgram(shaderId);
	glValidateProgram(shaderId);

	int valid;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &valid);
	if (!valid) {
		cout << "program shader error" << endl;
		return false;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);
	captureUniforms();
	return true;
}
bool Shader::compile(EShaderType eShaderType, const string &code, int &id) {
	id = glCreateShader(static_cast<unsigned>(eShaderType));
	const char *src = code.c_str();
	glDCall(glShaderSource(id, 1, &src, 0));
	glDCall(glCompileShader(id));

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *msg = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, msg);

		cout << (eShaderType == EShaderType::Vertex ? "vert" : "frag") << msg << endl;
		return false;
	}
	return true;
}

void Shader::clearUniforms() {
	for (auto &u : uniforms)
		delete u.second;
	uniforms.clear();
}
void Shader::captureUniforms() {
	clearUniforms();
	int uniformsSize = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &uniformsSize);
	for (int i = 0; i < uniformsSize; i++) {
		unsigned type;
		int len, size;
		char name[32] = "";
		glGetActiveUniform(shaderId, i, 32, &len, &size, &type, name);
		uniforms[name] = new ShaderParam(i, static_cast<EUniformType>(type));
	}
}
bool Shader::hasUniform(string name) {
	return uniforms.contains(name);
}
Inline void Shader::bind() {
	glUseProgram(shaderId);
}
Inline void Shader::unBind() const {
	glUseProgram(0);
}
Inline void Shader::uploadUniforms() {
	for (auto &u : uniforms)
		u.second->param->upload();
}

void Material::use() {
	shader->uploadUniforms();
	shader->bind();
}


bool ShaderFactory::assemble(Object *&content, Archive &ar) {
	string vertex, fragment;
	makeSourceFromArchive(ar, vertex, fragment);
	Shader *shader = new Shader;
	if (shader->create(vertex, fragment)) {
		content = shader;
		return true;
	}
	delete shader;
	shader = nullptr;
	return false;
}