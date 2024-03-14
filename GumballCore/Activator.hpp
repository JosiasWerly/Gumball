#pragma once
#ifndef _activator
#define _activator

#include "Patterns.hpp"
#include "picojson.h" //json library for now

#include <list>
#include <unordered_map>
#include <string>

class Field;
class FieldClass;
class FieldObject;
template<class TType> class FieldValue;
template<class TClass> class TFieldCtor;
class Class;

typedef unordered_map<string, Field *> Fields;
typedef unordered_map<string, FieldClass *> FObjects;

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
	template<class t> t* get(const intptr_t &obj) { return is<t>() ? dynamic_cast<FieldValue<t> *>(this)->get(obj) : nullptr; }

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

class FieldObject : public Field {
	template<class TClass> friend class TFieldCtor;
	FieldClass *objectClass;
public:
	FieldObject(FieldClass *objectClass) : objectClass(objectClass) {}
	FieldClass *getClass() { return objectClass; }

};

class FieldClassType {
	template<class TClass> friend class TFieldCtor;
public:
	virtual ~FieldClassType() = default;
	string name;
};

template<class TClass> class TFieldClassType : public FieldClassType {
public:
};

class FieldClass {
	template<class TClass> friend class TFieldCtor;
	FieldClassType *type;
	Fields fields;
public:

	FieldClass() = default;
	~FieldClass() {	delete type; }
	const string &getName() { return type->name; }
	Fields &getFields() { return fields; }
	template<class t> bool is() { return static_cast<bool>(dynamic_cast<TFieldClassType<t> *>(type)); }
};

class Property {
	intptr_t address;
	Field *field;
public:
	Property(intptr_t address, Field *field) : address(address), field(field) {}
	const string &getName() {
		return field->getName();
	}
	template<class t> bool is() {
		return field->is<t>();
	}
	template<class t> t *as() {
		return field->get<t>(address);
	}

	template<> bool is<Class>() {
		return static_cast<bool>(dynamic_cast<FieldObject*>(field));
	}
	
	Class asClass();
	operator bool() { return address != 0 && field; }

	SerialStream toStream();
	void fromStream(SerialStream stream);
};

class Class {
	intptr_t address;
	FieldClass *fclass;
public:
	Class() = default;
	~Class() = default;
	Class(intptr_t address, FieldClass *fclass) :
		address(address), 
		fclass(fclass) {
	}
	

	list<Property> getProperties() {
		Fields &fields = fclass->getFields();
		list<Property> out;
		for (auto kv : fields) {
			Field *field = kv.second;
			Property newProp(address + field->getAddress(), field);
			out.push_back(newProp);
		}
		return out;
	}
	const string &getName() {
		return fclass->getName();
	}
	template<class t> bool is() {
		return fclass->is<t>();
	}
	template<class t> t *as() {
		return is<t>() ? reinterpret_cast<t *>(address) : nullptr;
	}
	
	SerialStream toStream() {
		picojson::object jsonObject;
		picojson::object jfields;
		
		auto properties = getProperties();
		for (auto p : properties) {
			jfields[p.getName()] = p.toStream();
		}

		jsonObject["class"] = picojson::value(fclass->getName());
		jsonObject["fields"] = picojson::value(jfields);
		return SerialStream(picojson::value(jsonObject));
	}

	void fromStream(SerialStream stream) {
		JsonObject jobj = stream.getJsonValue().get<picojson::object>();

		if (fclass->getName() != jobj["class"].to_str()) {
			return;
		}

		JsonObject jfields = jobj["fields"].get<picojson::object>();
		auto properties = getProperties();
		for (auto p : properties) {
			if (jfields.contains(p.getName())) {
				p.fromStream(jfields[p.getName()]);
			}
		}
	}
	operator bool() { return address != 0 && fclass; }
};

class ImplClass {
public:
	Class getClass();
	virtual const string getClassName() { return ""; }
};

class GBCORE Activator : public Singleton<Activator> {
private:
	FObjects objects;

public:
	void add(FieldClass *object);
	void del(const string &name);
	FieldClass *get(const string &name);
};


template<class TClass> class TFieldCtor {
	typedef TFieldCtor Ctor;

	FieldClass *instance;
public:
	TFieldCtor(const string &name) {
		instance = new FieldClass;
		instance->type = new TFieldClassType<TClass>();
		instance->type->name = name;
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
		FieldObject *field = new FieldObject(nullptr);
		field->name = name;
		field->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
		field->objectClass = Activator::instance()->get(className);;
		instance->fields[name] = field;
		return *this;
	}

	operator FieldClass *() {
		FieldClass *out = instance;
		instance = nullptr;
		return out;
	}
};

#include "ActivatorProperties.hpp"
#endif //_activator