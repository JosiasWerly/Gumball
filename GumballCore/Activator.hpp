#pragma once
#ifndef _activator
#define _activator

#include "Patterns.hpp"
#include "picojson.h" //json library for now

#include <list>
#include <unordered_map>
#include <string>


namespace FieldSchema {
	class Field;
	class FieldObjectProxy;
	class FieldObject;
	template<class TType> class FieldValue;
	typedef unordered_map<string, Field *> Fields;
	typedef unordered_map<string, FieldObject *> FObjects;
};

class MetaObject;
class MetaValue;
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

class GBCORE Activator : public Singleton<Activator> {
private:
	FieldSchema::FObjects objects;

public:
	void add(FieldSchema::FieldObject *object);
	void del(const string &name);
	FieldSchema::FieldObject *get(const string &name);
};

namespace FieldSchema {

	class Field {
		template<class TClass> friend class TCtor;

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
		template<class TClass> friend class TCtor;

		FieldObject *objectClass;

	public:
		FieldObjectProxy(FieldObject *objectClass) : objectClass(objectClass) {}
		FieldObject *getMetaObject() { return objectClass; }
	};

	class FieldObject {
		template<class TClass> friend class TCtor;
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

	template<class TClass> class TCtor {
		typedef TCtor Ctor;

		FieldSchema::FieldObject *instance;
	public:
		TCtor(const string &name) {
			instance = new FieldObject;
			instance->name = name;
			instance->type = new FieldObject::TType<TClass>();
		}
		~TCtor() {
			delete instance;
		}

		template<class TProp>
		Ctor &prop(const string &name, TProp TClass:: *address) {
			FieldSchema::Field *field = new FieldSchema::FieldValue<TProp>;
			field->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
			field->name = name;
			instance->fields[name] = field;
			return *this;
		}

		template<class TProp>
		Ctor &prop(const string className, const string &name, TProp TClass:: *address) {
			FieldSchema::FieldObjectProxy *field = new FieldSchema::FieldObjectProxy(nullptr);
			field->name = name;
			field->address = reinterpret_cast<intptr_t>(&(((TClass *)nullptr)->*address));
			field->objectClass = Activator::instance()->get(className);;
			instance->fields[name] = field;
			return *this;
		}

		operator FieldObject *() {
			FieldSchema::FieldObject *out = instance;
			instance = nullptr;
			return out;
		}
	};
};

class MetaObject {
	intptr_t address;
	FieldSchema::FieldObject *object;

public:
	MetaObject() = default;
	~MetaObject() = default;
	MetaObject(intptr_t address, FieldSchema::FieldObject *metaObject) :
		address(address),
		object(metaObject) {
	}

	list<MetaValue> getValues();
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

class MetaValue {
	intptr_t address;
	FieldSchema::Field *field;

public:
	MetaValue(intptr_t address, FieldSchema::Field *field) : address(address), field(field) {}
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

class MetaClass {
public:
	virtual ~MetaClass() = default;

	MetaObject getMetaObject();
	virtual const string getClassName() = 0;
};

#include "ActivatorProperties.hpp"
#endif //_activator