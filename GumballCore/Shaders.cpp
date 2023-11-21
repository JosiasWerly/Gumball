#include "Shaders.hpp"
#include "Engine.hpp"
#include "Math.hpp"
#include "Texture.hpp"

IShaderUniformBus *IShaderUniformBus::TCreateBus(ShaderUniform &Uniform) {
	switch (Uniform.type) {
		case EUniformType::u_int:			return new TShaderUniformBus<int>(Uniform);				break;
		case EUniformType::u_float:			return new TShaderUniformBus<float>(Uniform);			break;
		case EUniformType::u_fvec2:			return new TShaderUniformBus<glm::fvec2>(Uniform);		break;
		case EUniformType::u_fvec3:			return new TShaderUniformBus<glm::fvec3>(Uniform);		break;
		case EUniformType::u_fvec4:			return new TShaderUniformBus<glm::fvec4>(Uniform);		break;
		case EUniformType::u_ivec2:			return new TShaderUniformBus<glm::ivec2>(Uniform);		break;
		case EUniformType::u_ivec3:			return new TShaderUniformBus<glm::ivec3>(Uniform);		break;
		case EUniformType::u_ivec4:			return new TShaderUniformBus<glm::ivec4>(Uniform);		break;
		case EUniformType::u_mat:			return new TShaderUniformBus<glm::mat4>(Uniform);		break;
		case EUniformType::u_stexture:		return new TShaderUniformBus<Tbo *>(Uniform);			break;
		default: throw;
	}
}

ShaderUniformIOBus::ShaderUniformIOBus() {
}
ShaderUniformIOBus::~ShaderUniformIOBus() {
	detach();
}
void ShaderUniformIOBus::attach(const list<ShaderUniform> &uniforms) {
	parameters.clear();
	for (const auto &u : uniforms) {
		if (u.type == EUniformType::u_stexture) {
			textures[u.name] = &u;
		}
		else {
			numbers[u.name] = &u;
		}
		parameters[u.name] = &u;
	}
}
void ShaderUniformIOBus::detach() {
	parameters.clear();
	textures.clear();
	parameters.clear();
}
void ShaderUniformIOBus::uploadNumbers() const {
	for (auto &kv : numbers)
		kv.second->bus->upload();
}
void ShaderUniformIOBus::uploadTextures() const {
	for (auto &kv : textures)
		kv.second->bus->upload();
}
void ShaderUniformIOBus::uploadParameters() const {
	for (auto &kv : parameters)
		kv.second->bus->upload();
}

Shader::Shader() {
}
Shader::~Shader() {
	uniformIOBus.detach();
	for (auto &u : uniforms)
		delete u.bus;
}
void Shader::updateUniforms() {
	unsigned texturesNum = 0;
	uniforms.clear();
	int uniformsSize = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &uniformsSize);
	for (int i = 0; i < uniformsSize; i++) {
		unsigned type;
		int len, size;
		char name[32] = "";
		glGetActiveUniform(shaderId, i, 32, &len, &size, &type, name);
		uniforms.push_back(
			{
				static_cast<unsigned>(i),
				static_cast<EUniformType>(type),
				name
			}
		);
		auto &newUniform = uniforms.back();
		newUniform.bus = IShaderUniformBus::TCreateBus(newUniform);

		if (newUniform.type == EUniformType::u_stexture) {
			auto textUni = dynamic_cast<TShaderUniformBus<Tbo*>*>(newUniform.bus);
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
	uniformIOBus.uploadParameters();
}

void ShaderInstance::setShader(Shader *newShader) {
	shader = newShader;
	if (shader) {
		uniformIOBus.attach(shader->getUniforms());
	}
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