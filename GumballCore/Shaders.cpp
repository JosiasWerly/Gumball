#include "Shaders.hpp"
#include "Engine.hpp"
#include "Math.hpp"
#include "Texture.hpp"


Shader::Shader() {
}
Shader::~Shader() {
}
void Shader::updateUniforms() {
	uniforms.clear();

	unsigned texturesNum = 0;
	int uniformsSize = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &uniformsSize);
	for (int i = 0; i < uniformsSize; i++) {
		unsigned type;
		int len, size;
		char name[32] = "";
		glGetActiveUniform(shaderId, i, 32, &len, &size, &type, name);

		//TODO: check if ShaderUniform::Add is correcly storing shaderInfo, it shouldn't based on the lifetime of this variable
		ShaderUniformInfo shaderInfo {
			static_cast<unsigned>(i),
			static_cast<EUniformType>(type),
			name
		};
		ShaderUniformBus* bus =  uniforms.add(shaderInfo);
		if (bus->owner.type == EUniformType::u_stexture) {
			auto textUni = dynamic_cast<TShaderUniformBus<Tbo*>*>(bus);
			textUni->activeId = texturesNum++;			
		}
	}
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
	updateUniforms();
	return true;
}
void Shader::bind() const {
	glUseProgram(shaderId);
}
void Shader::unbind() const {
	glUseProgram(0);
}
void Shader::upload() const {
	uniforms.uploadAll();
}

void ShaderInstance::setShader(Shader *newShader) {
	shader = newShader;
	params.setParent(&newShader->getUniforms());
}

AssetFactory<Shader>::AssetFactory() {
	extensions = { "shader", "glsl" };
}
bool AssetFactory<Shader>::load(Archive &ar, Shader &val) {
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

	if (val.create(vertex, fragment))
		return true;
	return false;
}
bool AssetFactory<Shader>::save(Archive &ar, const Shader &val) {
	throw;
	return false;
}