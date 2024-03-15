#pragma once
#ifndef _activator
#define _activator

#include "Patterns.hpp"
#include "picojson.h" //json library for now

#include <list>
#include <unordered_map>
#include <string>

class Field;
class FieldObjectProxy;
class FieldObject;
template<class TType> class FieldValue;
template<class TClass> class TFieldCtor;

class MetaObject;
class MetaField;

typedef unordered_map<string, Field *> Fields;
typedef unordered_map<string, FieldObject *> FObjects;

typedef picojson::object JsonObject;
typedef picojson::value JsonValue;

class SerialStream {
	JsonValue jvalue;
public:
	SerialStream() {}
	SerialStream(JsonValue jvalue) : jvalue(jvalue) {}
	string toString() const { return jvalue.serialize(true); }
	JsonValue getJsonValue() const { return jvalue; }
	operator JsonValue &() { return jvalue; }
};

class Field {
	template<class TClass> friend class TFieldCtor;

protected:
	intptr_t address;
	string name;

public:
	virtual ~Field() {};
	const string &getName() { return name; }
	const intptr_t &getAddress() { return address; }

	template<class t> bool is() { return static_cast<bool>(dynamic_cast<FieldValue<t> *>(this)); }
	template<class t> t *get(const intptr_t &obj) { return is<t>() ? dynamic_cast<FieldValue<t> *>(this)->get(obj) : nullptr; }

	virtual SerialStream toStream(const intptr_t &obj) { return SerialStream(); }
	virtual void fromStream(const intptr_t &obj, SerialStream &stream) {}
};

template<class TType> class FieldValue : public Field {
public:
	TType *get(const intptr_t &obj) { return reinterpret_cast<TType *>(obj); }

	SerialStream toStream(const intptr_t &obj) {
		return JsonValue(string(reinterpret_cast<char *>(get(obj)), sizeof(TType)));
	}
	void fromStream(const intptr_t &obj, SerialStream &stream) {
		std::memcpy(get(obj), stream.getJsonValue().to_str().c_str(), sizeof(TType));
	}
};

class FieldObjectProxy : public Field {
	template<class TClass> friend class TFieldCtor;

	FieldObject *objectClass;

public:
	FieldObjectProxy(FieldObject *objectClass) : objectClass(objectClass) {}
	FieldObject *getMetaObject() { return objectClass; }
};

class FieldObject {
	template<class TClass> friend class TFieldCtor;
public:
	class Type {
		template<class TClass> friend class TFieldCtor;
	public:
		virtual ~Type() = default;
	};
	template<class TClass> class TType : public Type {
	public:
	};

private:
	Type *type;
	Fields fields;
	string name;

public:
	FieldObject() = default;
	~FieldObject() { delete type; }
	const string &getName() { return name; }
	Fields &getFields() { return fields; }
	template<class t> bool is() { return static_cast<bool>(dynamic_cast<TType<t> *>(type)); }
};

class MetaObject {
	intptr_t address;
	FieldObject *object;

public:
	MetaObject() = default;
	~MetaObject() = default;
	MetaObject(intptr_t address, FieldObject *metaObject) :
		address(address),
		object(metaObject) {
	}

	list<MetaField> getFields();
	SerialStream toStream();
	void fromStream(SerialStream stream);

	const string &getName() {
		return object->getName();
	}
	template<class t> bool is() {
		return object->is<t>();
	}
	template<class t> t *as() {
		return is<t>() ? reinterpret_cast<t *>(address) : nullptr;
	}

	operator bool() { return address != 0 && object; }
};

class MetaField {
	intptr_t address;
	Field *field;

public:
	MetaField(intptr_t address, Field *field) : address(address), field(field) {}
	bool isObject();
	MetaObject asObject();
	SerialStream toStream();
	void fromStream(SerialStream stream);

	const string &getName() {
		return field->getName();
	}
	template<class t> bool is() {
		return field->is<t>();
	}
	template<class t> t *as() {
		return field->get<t>(address);
	}

	operator bool() { return address != 0 && field; }
};


class ImplClass {
public:
	MetaObject getClass();
	virtual const string getClassName() { return ""; }
};

class GBCORE Activator : public Singleton<Activator> {
private:
	FObjects objects;

public:
	void add(FieldObject *object);
	void del(const string &name);
	FieldObject *get(const string &name);
};


template<class TClass> class TFieldCtor {
	typedef TFieldCtor Ctor;

	FieldObject *instance;
public:
	TFieldCtor(const string &name) {
		instance = new FieldObject;
		instance->name = name;
		instance->type = new FieldObject::TType<TClass>();
	}
	~TFieldCtor() {
		delete instance;
	}
	template<class TProp>
	Ctor &prop(const string &name, TProp TClass:: *address) {
		Field *field = new FieldValue<TProp>;
		field->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
		field->name = name;
		instance->fields[name] = field;
		return *this;
	}

	template<class TProp>
	Ctor &prop(const string className, const string &name, TProp TClass:: *address) {
		FieldObjectProxy *field = new FieldObjectProxy(nullptr);
		field->name = name;
		field->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
		field->objectClass = Activator::instance()->get(className);;
		instance->fields[name] = field;
		return *this;
	}

	operator FieldObject *() {
		FieldObject *out = instance;
		instance = nullptr;
		return out;
	}
};

#include "ActivatorProperties.hpp"
#endif //_activator