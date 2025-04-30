#include "Shaders.hpp"
#include "Texture.hpp"
#include "GLUtils.hpp"

#include <fstream>
#include <sstream>

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
		ShaderUniformInfo shaderInfo{
			static_cast<unsigned>(i),
			static_cast<EUniformType>(type),
			name
		};

		ShaderUniformBus *bus = uniforms.add(shaderInfo);
		if (bus->info.type == EUniformType::utexture) {
			TShaderUniformBus<Texture*> *textUni = dynamic_cast<TShaderUniformBus<Texture*>*>(bus);
			textUni->activeId = texturesNum++;
		}
	}
}
bool Shader::compile(EShaderType eShaderType, const string &code, int &id) {
	unsigned shaderType;
	switch (eShaderType) {
		case Shader::EShaderType::Vertex:	shaderType = GL_VERTEX_SHADER; break;
		case Shader::EShaderType::Fragment:	shaderType = GL_FRAGMENT_SHADER; break;
	}
	id = glCreateShader(shaderType);
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

bool WFileSerializer<Shader>::load(const FilePath &fp, Shader &val) {
	string vertex, fragment;
	Archive ar(fp.path());
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
	if (vertex.empty() || fragment.empty()) {
		return false;
	}

	if (val.create(vertex, fragment)) {
		return true;
	}
	return false;
}
bool WFileSerializer<Shader>::save(const FilePath &fp, Shader &val) {
	throw;
	return false;
}
bool WFileSerializer<Shader>::supports(const FilePath &fp) const {
	return fp.path() == "shader";
}