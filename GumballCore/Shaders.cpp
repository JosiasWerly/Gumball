#include "Shaders.hpp"
#include "Engine.hpp"
#include "Math.hpp"
#include "Texture.hpp"

ShaderAttribute::ShaderAttribute(unsigned location, EUniformType type) :
	location(location),
	type(type) {

	switch (type) {
	case EUniformType::u_int:			param = new ShaderParameter<EUniformType::u_int>(*this);			break;
	case EUniformType::u_float:			param = new ShaderParameter<EUniformType::u_float>(*this);		break;
	case EUniformType::u_mat:			param = new ShaderParameter<EUniformType::u_mat>(*this);			break;
	case EUniformType::u_stexture:		param = new ShaderParameter<EUniformType::u_stexture>(*this);	break;
	case EUniformType::u_fvec2:			param = new ShaderParameter<EUniformType::u_fvec2>(*this);		break;
	case EUniformType::u_fvec3:			param = new ShaderParameter<EUniformType::u_fvec3>(*this);		break;
	case EUniformType::u_fvec4:			param = new ShaderParameter<EUniformType::u_fvec4>(*this);		break;
	case EUniformType::u_ivec2:			param = new ShaderParameter<EUniformType::u_ivec2>(*this);		break;
	case EUniformType::u_ivec3:			param = new ShaderParameter<EUniformType::u_ivec3>(*this);		break;
	case EUniformType::u_ivec4:			param = new ShaderParameter<EUniformType::u_ivec4>(*this);		break;
	default: throw;
	}
}
ShaderAttribute::~ShaderAttribute() {
}


Shader::Shader() {
}
Shader::~Shader() {
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
	captureAttributeSchema();
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
void Shader::captureAttributeSchema() {
	attributeScheme.clear();
	int uniformsSize = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &uniformsSize);
	for (int i = 0; i < uniformsSize; i++) {
		unsigned type;
		int len, size;
		char name[32] = "";
		glGetActiveUniform(shaderId, i, 32, &len, &size, &type, name);
		attributeScheme.push_back({
				name,
				static_cast<unsigned>(i),
				static_cast<EUniformType>(type)
								  }
		);
	}
}
void Shader::bind() const {
	glUseProgram(shaderId);
}
void Shader::unbind() const {
	glUseProgram(0);
}
bool Shader::archiveLoad(Archive &ar) {
	string vertex, fragment;
	{
		enum class ESType {
			none = -1, vertex, fragment
		} shType;
		string line;
		while (ar.getLine(line)) {
			if (line.find("#vert") != string::npos) {
				shType = ESType::vertex;
			}
			else if (line.find("#frag") != string::npos) {
				shType = ESType::fragment;
			}
			else if (line.find("//") == string::npos) {
				if (shType == ESType::vertex)
					vertex += line + "\n";
				else
					fragment += line + "\n";
			}
		}
	}
	if (vertex.empty() || fragment.empty())
		return false;

	if (create(vertex, fragment))
		return true;
	return false;
}
bool Shader::archiveSave(Archive &ar) {
	return false;
}



ShaderInstance::ShaderInstance(Shader *sh) {
	if (!sh) {
		throw;
	}
	shader = sh;
	copyParameters();
}
ShaderInstance::~ShaderInstance() {
	clearParameters();
}
void ShaderInstance::copyParameters() {
	clearParameters();
	auto &attributes = shader->getAttributes();
	for (auto &a : attributes)
		parameters[a.name] = new ShaderAttribute(a.location, a.type);
}
void ShaderInstance::clearParameters() {
	for (auto &u : parameters)
		delete u.second;
	parameters.clear();
}
void ShaderInstance::upload() {
	for (auto &u : parameters)
		u.second->param->upload();
}

//Inline void Material::copyParameters() {
//	if (!shader)
//		return;
//	clearParameters();
//
//	auto &attributes = shader->getAttributes();
//	for (auto &a : attributes)
//		parameters[a.name] = new ShaderAttribute(a.location, a.type);
//}
//Inline void Material::clearParameters() {
//	for (auto &u : parameters)
//		delete u.second;
//	parameters.clear();
//}
//Material::Material() {
//}
//Material::~Material() {
//	clearParameters();
//}
//void Material::setShader(Shader *shader) {
//	this->shader = shader;
//	copyParameters();
//}
//void Material::bind() {
//	shader->bind();
//}
//void Material::unBind() {
//	shader->unBind();
//}
//void Material::uploadParams() {
//	for (auto &u : parameters)
//		u.second->param->upload();
//}