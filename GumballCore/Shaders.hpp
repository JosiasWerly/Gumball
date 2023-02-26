#pragma once
#ifndef _shaders
#define _shaders
#include "AssetManager.hpp"
#include "GLUtils.hpp"
#include "Math.hpp"
#include "Texture.hpp"

#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
using namespace std;

class IShaderParameter;


enum class EUniformType {
	u_int = GL_INT,
	u_float = GL_FLOAT,
	u_mat = GL_FLOAT_MAT4,
	u_stexture = GL_SAMPLER_2D,
	u_fvec2 = GL_FLOAT_VEC2,
	u_fvec3 = GL_FLOAT_VEC3,
	u_fvec4 = GL_FLOAT_VEC4,
	u_ivec2 = GL_INT_VEC2,
	u_ivec3 = GL_INT_VEC3,
	u_ivec4 = GL_INT_VEC4,
};

struct ShaderAttributeDescriptor {
	unsigned location;
	EUniformType type;
	string name;
	ShaderAttributeDescriptor(string name, unsigned location, EUniformType type) :
		name(name),
		location(location),
		type(type) {
	}
};

class GBCORE Shader :
	public Object {
public:
	enum class EShaderType {
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};
	
protected:
	list<ShaderAttributeDescriptor> attributeScheme;

	bool compile(EShaderType eShaderType, const string& code, int& id);
	void captureAttributeSchema();
public:
	virtual Var<Object> clone() const override { return Var<Object>(new Shader); }
	virtual bool archiveLoad(Archive &ar) override;
	virtual bool archiveSave(Archive &ar) override;
	
	unsigned shaderId = 0;

	Shader();
	virtual ~Shader();
	bool create(const string &vertex, const string &fragment);

	Inline void bind() const;
	Inline void unbind() const;
	
	Inline const list<ShaderAttributeDescriptor> &getAttributes() const { return attributeScheme; }
	bool hasAttribute(string name) { 
		for (auto &a : attributeScheme) {
			if (a.name == name)
				return true;
		}
		return false;
	}
	const ShaderAttributeDescriptor* getAttribute(string name) {
		for (auto &a : attributeScheme) {
			if (a.name == name)
				return &a;
		}
		return nullptr;
	}
};


class ShaderAttribute {
public:
	const unsigned location;
	const EUniformType type;
	IShaderParameter *param;

	ShaderAttribute(unsigned location, EUniformType type);
	virtual ~ShaderAttribute();
};
class IShaderParameter {
public:
	const ShaderAttribute &owner;
	IShaderParameter(const ShaderAttribute &owner) : owner(owner){}
	virtual void upload() = 0;
};

template<class T> 
class TShaderParameter : 
	public IShaderParameter { 
public:
	using IShaderParameter::IShaderParameter;
	virtual void upload() = 0;
};
#define ShaderParamDelc(CType, Exp)\
template<> class TShaderParameter<CType> : public IShaderParameter {\
public:\
	using IShaderParameter::IShaderParameter;\
	typedef CType Type;\
	CType val;\
	void upload() override { Exp; } \
}
ShaderParamDelc(int,			glUniform1iv(owner.location, 1, &val));
ShaderParamDelc(float,			glUniform1fv(owner.location, 1, &val));
ShaderParamDelc(glm::fvec2,		glUniform2fv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::fvec3,		glUniform3fv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::fvec4,		glUniform4fv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::ivec2,		glUniform2iv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::ivec3,		glUniform3iv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::ivec4,		glUniform4iv(owner.location, 1, glm::value_ptr(val)));
ShaderParamDelc(glm::mat4,		glUniformMatrix4fv(owner.location, 1, GL_FALSE, &val[0][0]));
ShaderParamDelc(Image *,		glActiveTexture(GL_TEXTURE0); val->bind(););
#undef ShaderParamDelc


class GBCORE Material {
private:
	Shader *shader;
	unordered_map<string, ShaderAttribute *> parameters;

	void copyParameters();
	void clearParameters();

	template<class T> TShaderParameter<T> *paramAs(const string &name) {
		
		return dynamic_cast<TShaderParameter<T>*>(parameters[name]->param);
	}
public:
	Material(Shader *sh);
	~Material();

	Inline void bind() {
		shader->bind();
	}
	Inline void unbind() {
		shader->unbind();
	}
	Shader *getShader() { 
		return shader; 
	}

	template<class T> void setParam(const string &name, T) = delete;
	template<class T> T getParam(const string &name) = delete;

	template<> void setParam<Vector3>(const string &name, Vector3 value) {
		auto &p = *paramAs<glm::fvec3>(name);
		p.val = value.rawVector;
		p.upload();
	}
	template<> Vector3 getParam <Vector3>(const string &name) {
		const auto &v = paramAs<glm::fvec3>(name)->val;
		return Vector3(v.x, v.y, v.z);
	}

	template<> void setParam<Color>(const string &name, Color value) {
		auto &p = *paramAs<glm::fvec4>(name);
		p.val = Color::toVec4(value);
		p.upload();
	}
	template<> Color getParam <Color>(const string &name) {
		const auto &v = paramAs<glm::fvec4>(name)->val;
		return Color::toColor(v);
	}

	#define TParamDelc(CType)\
	template<> void setParam<CType>(const string &name, CType value) { \
		auto &p = *paramAs<CType>(name);\
		p.val = value;\
		p.upload();\
	}\
	template<> CType getParam<CType>(const string &name) { \
		return paramAs<CType>(name)->val;\
	}

	TParamDelc(float);
	TParamDelc(int);
	TParamDelc(glm::mat4);
	TParamDelc(Image*);
	#undef TParamDelc
};

//class IShaderParameter {
//public:
//	const ShaderAttribute &owner;
//	IShaderParameter(const ShaderAttribute &owner) : owner(owner) {}
//	virtual void upload() = 0;
//};
//template<EUniformType Type>
//class TShaderParameter :
//	public IShaderParameter {
//private:
//	TShaderParameter() = delete
//		void upload() override {}
//};
//#define ShaderParamDelc(EType, CType, Exp)\
//template<> class TShaderParameter<EType> : public IShaderParameter {\
//public:\
//	using IShaderParameter::IShaderParameter;\
//	typedef CType Type;\
//	CType val;\
//	void upload() override { Exp; } \
//}
//ShaderParamDelc(EUniformType::u_int, int, glUniform1iv(owner.location, 1, &val));
//ShaderParamDelc(EUniformType::u_float, float, glUniform1fv(owner.location, 1, &val));
//ShaderParamDelc(EUniformType::u_mat, glm::mat4, glUniformMatrix4fv(owner.location, 1, GL_FALSE, &val[0][0]));
//ShaderParamDelc(EUniformType::u_fvec2, glm::fvec2, glUniform2fv(owner.location, 1, glm::value_ptr(val)));
//ShaderParamDelc(EUniformType::u_fvec3, glm::fvec3, glUniform3fv(owner.location, 1, glm::value_ptr(val)));
//ShaderParamDelc(EUniformType::u_fvec4, glm::fvec4, glUniform4fv(owner.location, 1, glm::value_ptr(val)));
//ShaderParamDelc(EUniformType::u_ivec2, glm::ivec2, glUniform2iv(owner.location, 1, glm::value_ptr(val)));
//ShaderParamDelc(EUniformType::u_ivec3, glm::ivec3, glUniform3iv(owner.location, 1, glm::value_ptr(val)));
//ShaderParamDelc(EUniformType::u_ivec4, glm::ivec4, glUniform4iv(owner.location, 1, glm::value_ptr(val)));
//ShaderParamDelc(EUniformType::u_stexture, Image *, glActiveTexture(GL_TEXTURE0); /*val->bind();*/);
//#undef ShaderParamDelc
//
//
//class GBCORE Material {
//private:
//	Shader *shader;
//	unordered_map<string, ShaderAttribute *> parameters;
//
//	void copyParameters();
//	void clearParameters();
//public:
//	Material(Shader *sh);
//	~Material();
//
//	Inline void bind() {
//		shader->bind();
//	}
//	Inline void unbind() {
//		shader->unbind();
//	}
//	Shader *getShader() { return shader; }
//
//	template<class T> void setParam(const string &name, T) = delete;
//	template<class T> T getParam(const string &name) = delete;
//
//	template<> void setParam<Vector3>(const string &name, Vector3 value) {
//		auto &p = parameters[name]->get<EUniformType::u_fvec3>();
//		p.val = value.rawVector;
//		p.upload();
//	}
//	template<> Vector3 getParam <Vector3>(const string &name) {
//		const auto &v = &parameters[name]->get<EUniformType::u_fvec3>().val;
//		return Vector3(v->x, v->y, v->z);
//	}
//
//	template<> void setParam<Color>(const string &name, Color value) {
//		auto &p = parameters[name]->get<EUniformType::u_fvec4>();
//		p.val = value;
//		p.upload();
//	}
//	template<> Color getParam <Color>(const string &name) {
//		auto &p = parameters[name]->get<EUniformType::u_fvec4>();
//		return Color(p.val.r, p.val.g, p.val.b, p.val.a);
//	}
//
//	#define TParamDelc(EType, CType)\
//	template<> void setParam<CType>(const string &name, CType value) { \
//		auto &p = parameters[name]->get<EType>();\
//		p.val = value;\
//		p.upload();\
//	}\
//	template<> CType getParam<CType>(const string &name) { \
//		return parameters[name]->get<EType>().val;\
//	}
//
//	TParamDelc(EUniformType::u_float, float);
//	TParamDelc(EUniformType::u_int, int);
//	TParamDelc(EUniformType::u_mat, glm::mat4);
//	TParamDelc(EUniformType::u_stexture, Image *);
//	#undef TParamDelc
//};

//class IShaderParameter {
//	friend class Shader;
//	friend class ShaderInstance;
//protected:
//	const ShaderAttribute &owner;
//public:
//	IShaderParameter(const ShaderAttribute &owner) : 
//		owner(owner) {
//	}
//	virtual void upload() = 0;
//};
//template<EUniformType Type> class ShaderParameter :
//	public IShaderParameter {
//protected:
//	using IShaderParameter::upload;
//public:
//	using IShaderParameter::IShaderParameter;
//};
//  
//#define ShaderParamDelc(Type, ValueType, Expression) \
//template<> class ShaderParameter<Type> : \
//	public IShaderParameter {\
//public:\
//	typename typedef ValueType TType;\
//	ValueType value = ValueType();\
//	using IShaderParameter::IShaderParameter;\
//	ValueType& operator=(ValueType &other){ value = other; return value;}\
//	void upload() override { Expression; }\
//};
//
//ShaderParamDelc(EUniformType::u_int, int, glUniform1iv(owner.location, 1, &value))
//ShaderParamDelc(EUniformType::u_float, float, glUniform1fv(owner.location, 1, &value));
//ShaderParamDelc(EUniformType::u_mat, glm::mat4, glUniformMatrix4fv(owner.location, 1, GL_FALSE, &value[0][0]));
//ShaderParamDelc(EUniformType::u_fvec2, glm::fvec2, glUniform2fv(owner.location, 1, glm::value_ptr(value)));
//ShaderParamDelc(EUniformType::u_fvec3, glm::fvec3, glUniform3fv(owner.location, 1, glm::value_ptr(value)));
//ShaderParamDelc(EUniformType::u_fvec4, glm::fvec4, glUniform4fv(owner.location, 1, glm::value_ptr(value)));
//ShaderParamDelc(EUniformType::u_ivec2, glm::ivec2, glUniform2iv(owner.location, 1, glm::value_ptr(value)));
//ShaderParamDelc(EUniformType::u_ivec3, glm::ivec3, glUniform3iv(owner.location, 1, glm::value_ptr(value)));
//ShaderParamDelc(EUniformType::u_ivec4, glm::ivec4, glUniform4iv(owner.location, 1, glm::value_ptr(value)));
//
//template<> class ShaderParameter<EUniformType::u_stexture> : 
//	public IShaderParameter {
//protected:
//	void upload() override {
//		if (!image)
//			return;
//		glActiveTexture(GL_TEXTURE0 + slot);
//		image->bind();
//	}
//public:
//	using IShaderParameter::IShaderParameter;
//	unsigned slot = 0;
//	Image *image;
//};
//#undef ShaderParamDelc
//
//
//
//class GBCORE ShaderInstance {
//private:
//	Shader *shader;
//	unordered_map<string, ShaderAttribute *> parameters;
//
//	void copyParameters();
//	void clearParameters();
//public:
//	ShaderInstance(Shader *sh);
//	~ShaderInstance();
//
//	Inline Shader *getShader() const { return shader; }
//	Inline void bind() const { shader->bind(); }
//	Inline void unbind() const { shader->unbind(); }
//	Inline bool operator==(Shader *s) const { return shader == s; }
//	Inline bool operator==(ShaderInstance *si) const { return shader == si->shader; }
//	void upload();
//
//	template<EUniformType eType>
//	ShaderParameter<eType> *param(const string &name) {
//		return dynamic_cast<ShaderParameter<eType>*>(parameters[name]->param);
//	}
//
//	template<EUniformType eType, class T>
//	typename void param(const string &name, const T &param) {
//
//	}
//
//	#define ShaderParamDelc(Type)\
//	template<>\
//	void param<Type>(const string &name, const ShaderParameter<Type>::TType &value) {\
//		ShaderParameter<Type> *p = param<Type>(name);\
//		p->value = value;\
//		p->upload();\
//	}
//	ShaderParamDelc(EUniformType::u_int)
//	ShaderParamDelc(EUniformType::u_float);
//	ShaderParamDelc(EUniformType::u_mat);
//	ShaderParamDelc(EUniformType::u_fvec2);
//	ShaderParamDelc(EUniformType::u_fvec3);
//	ShaderParamDelc(EUniformType::u_fvec4);
//	ShaderParamDelc(EUniformType::u_ivec2);
//	ShaderParamDelc(EUniformType::u_ivec3);
//	ShaderParamDelc(EUniformType::u_ivec4);
//	#undef ShaderParamDelc
//};
#endif // !_shaders