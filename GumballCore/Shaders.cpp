#include "Shaders.hpp"
#include "Engine.hpp"
#include "Math.hpp"

bool ShaderFactory::assemble(Asset &asset, Archive &ar) {
	string vertex, fragment;
	makeSourceFromArchive(ar, vertex, fragment);
	Shader *shader = new Shader;
	if (shader->create(vertex, fragment)) {
		asset << shader;
		return true;
	}
	delete shader;
	return false;
}

template<typename T>
class UniformIO :
	public IShaderParamIO {
	using IShaderParamIO::upload;
	using IShaderParamIO::getData;
};

//specific case
#define UniformIODelc(Type, Expression) \
template<> class UniformIO<Type> : public IShaderParamIO { \
public: \
	Type value = Type(); \
	using IShaderParamIO::IShaderParamIO; \
	void upload() override { Expression; } \
	void* getData() override { return &value; } \
}; 

UniformIODelc(int, glUniform1iv(owner.location, 1, &value))
UniformIODelc(float, glUniform1fv(owner.location, 1, &value));
UniformIODelc(glm::mat4, glUniformMatrix4fv(owner.location, 1, GL_FALSE, &value[0][0]));
UniformIODelc(glm::fvec2, glUniform2fv(owner.location, 1, glm::value_ptr(value)));
UniformIODelc(glm::fvec3, glUniform3fv(owner.location, 1, glm::value_ptr(value)));
UniformIODelc(glm::fvec4, glUniform4fv(owner.location, 1, glm::value_ptr(value)));
UniformIODelc(glm::ivec2, glUniform2iv(owner.location, 1, glm::value_ptr(value)));
UniformIODelc(glm::ivec3, glUniform3iv(owner.location, 1, glm::value_ptr(value)));
UniformIODelc(glm::ivec4, glUniform4iv(owner.location, 1, glm::value_ptr(value)));


ShaderParam::ShaderParam(unsigned location, unsigned type) :
	location(location),
	type(type) {

	switch (type) {
	case GL_INT:            paramIO = new UniformIO<int>(*this); break;
	case GL_FLOAT:          paramIO = new UniformIO<float>(*this); break;
	case GL_SAMPLER_2D:     paramIO = new UniformIO<int>(*this); break;
	case GL_FLOAT_VEC2:     paramIO = new UniformIO<glm::fvec2>(*this); break;
	case GL_FLOAT_VEC3:     paramIO = new UniformIO<glm::fvec3>(*this); break;
	case GL_FLOAT_VEC4:     paramIO = new UniformIO<glm::fvec4>(*this); break;
	case GL_INT_VEC4:       paramIO = new UniformIO<glm::ivec4>(*this); break;
	case GL_INT_VEC3:       paramIO = new UniformIO<glm::ivec3>(*this); break;
	case GL_INT_VEC2:       paramIO = new UniformIO<glm::ivec2>(*this); break;
	case GL_FLOAT_MAT4:     paramIO = new UniformIO<glm::mat4>(*this); break;
	default: throw; //not found;
	}
}
ShaderParam::~ShaderParam() {
	delete paramIO;
}

void ShaderParameters::clearUniforms() {
	for (auto &u : uniforms)
		delete u.second;
	uniforms.clear();
}
void ShaderParameters::captureUniforms() {
	clearUniforms();
	int uniformsSize = 0;
	glGetProgramiv(owner.shaderId, GL_ACTIVE_UNIFORMS, &uniformsSize);
	for (int i = 0; i < uniformsSize; i++) {
		unsigned type;
		int len, size;
		char name[32] = "";
		glGetActiveUniform(owner.shaderId, i, 32, &len, &size, &type, name);
		uniforms[name] = new ShaderParam(i, type);
	}
}
void ShaderParameters::uploadUniforms() {
	for (auto &u : uniforms)
		u.second->paramIO->upload();
}

Shader::Shader() :
	parameters(*this) {
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
	parameters.captureUniforms();
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
Inline void Shader::bind() {
	glUseProgram(shaderId);
}
Inline void Shader::unBind() const {
	glUseProgram(0);
}
Inline void Shader::uploadUniforms() {
	parameters.uploadUniforms();
}


void Material::use() {
	shader->bind();
	shader->uploadUniforms();

}
bool Material::setShader(string name) {

	auto &assetSys = *Engine::instance()->getSystem<AssetsSystem>();
	Shader *sh = nullptr;
	if (assetSys(name, sh))
		shader = sh;
	return shader;
}
bool Material::isInstance() { return false; }
