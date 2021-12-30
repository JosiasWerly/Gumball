#include "Shaders.hpp"
#include "Math.hpp"

bool ShaderFactory::assemble(Asset &asset, Archive &ar) {
	string vertex, fragment;
	makeSourceFromArchive(ar, vertex, fragment);
	Shader* shader = new Shader;
	if (shader->create(vertex, fragment)) {
		asset << shader;
		return true;
	}

	return false;
}

template<class T> 
class Uniform : 
	public ShaderParamBind {
public:
	using ShaderParamBind::upload;
	Uniform() = delete;
};

#define UniformDecl(Type, Exp)\
template<> class Uniform<Type> : public ShaderParamBind {\
    public:\
    Type data = Type();\
    void upload() override { Exp; }\
	Uniform(){}
};

UniformDecl(int, glUniform1iv(location, 1, &data));
UniformDecl(float, glUniform1fv(location, 1, &data));
UniformDecl(glm::mat4, glUniformMatrix4fv(location, 1, GL_FALSE, &data[0][0]));
UniformDecl(glm::fvec2, glUniform2fv(location, 1, glm::value_ptr(data)));
UniformDecl(glm::fvec3, glUniform3fv(location, 1, glm::value_ptr(data)));
UniformDecl(glm::fvec4, glUniform4fv(location, 1, glm::value_ptr(data)));
UniformDecl(glm::ivec2, glUniform2iv(location, 1, glm::value_ptr(data)));
UniformDecl(glm::ivec3, glUniform3iv(location, 1, glm::value_ptr(data)));
UniformDecl(glm::ivec4, glUniform4iv(location, 1, glm::value_ptr(data)));
#undef UniformDecl

static iParamStorage* reflectGLEnum(GLenum type) {
	
}

void ShaderParameters::updateUniforms() {
	uniforms.clear();
	int uniformsSize = 0;
	glGetProgramiv(owner.shaderId, GL_ACTIVE_UNIFORMS, &uniformsSize);
	for (int i = 0; i < uniformsSize; i++) {
		unsigned type;
		char name[32] = "";
		glGetActiveUniform(owner.shaderId, i, 32, nullptr, nullptr, &type, name);
		uniforms.emplace(name, ShaderParamBind(i, type));
	}
}
ShaderParamBind* ShaderParameters::newShaderBind(unsigned type) {
	switch (type) {
	case GL_FLOAT:          return new Uniform<float>;
	case GL_INT:            return new Uniform<int>;
	case GL_SAMPLER_2D:     return new Uniform<int>;
	case GL_FLOAT_VEC2:     return new Uniform<glm::fvec2>;
	case GL_FLOAT_VEC3:     return new Uniform<glm::fvec3>;
	case GL_FLOAT_VEC4:     return new Uniform<glm::fvec4>;
	case GL_INT_VEC4:       return new Uniform<glm::ivec4>;
	case GL_INT_VEC3:       return new Uniform<glm::ivec3>;
	case GL_INT_VEC2:       return new Uniform<glm::ivec2>;
	case GL_FLOAT_MAT4:     return new Uniform<glm::mat4>; 
	default:
		cout << "reflectGLEnumError" << endl;
		throw;
	}
	return out;
}
void ShaderParameters::clearUniform() {

}
//void ShaderParameters::updateAttributes() {
//	attributes.clear();
//	int uniformsSize = 0;
//	glGetProgramiv(owner.shaderId, GL_ACTIVE_ATTRIBUTES, &uniformsSize);
//	for (int i = 0; i < uniformsSize; i++) {
//		unsigned type;
//		char name[32] = "";
//		glGetActiveAttrib(owner.shaderId, i, 32, nullptr, nullptr, &type, name);
//		attributes.emplace(name, ShaderParamBind(i, type));
//	}
//}

bool Shader::create(const string& vertex, const string& fragment) {
	int	p = glCreateProgram(),
		vs, fs;

	if (!compile(EShaderType::Vertex, vertex, vs))
		return false;
	if (!compile(EShaderType::Fragment, fragment, fs))
		return false;

	glAttachShader(p, vs);
	glAttachShader(p, fs);
	glLinkProgram(p);
	glValidateProgram(p);

	int valid;
	glGetProgramiv(p, GL_LINK_STATUS, &valid);
	if (!valid) {
		cout << "program shader error" << endl;
		return false;
	}
	glDeleteShader(vs);
	glDeleteShader(fs);

	updateUniforms();
	updateAttributes();
	return true;
}
bool Shader::compile(EShaderType eShaderType, const string& code, int& id) {
	id = glCreateShader(static_cast<unsigned>(eShaderType));
	const char* src = code.c_str();
	glDCall(glShaderSource(id, 1, &src, 0));
	glDCall(glCompileShader(id));

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, msg);

		cout << (eShaderType == EShaderType::Vertex ? "vert" : "frag") << msg << endl;
		return false;
	}
	return true;
}
