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
class MetaClass;
typedef picojson::object JsonObject;
typedef picojson::value JsonValue;


class GBCORE Activator : public Singleton<Activator> {
private:
	FieldSchema::FObjects objects;

public:
	void add(FieldSchema::FieldObject *object);
	void del(const string &name);
	FieldSchema::FieldObject *get(const string &name);
};

class SerialStream {
	JsonValue jvalue;
	
	void *toMetaClass();
public:
	SerialStream() {}
	SerialStream(JsonValue jvalue) : jvalue(jvalue) {}
	string toString(bool pretty = false) const;

	template<class t> t *toMetaClass() { 
		return reinterpret_cast<t *>(toMetaClass());
	}
	JsonValue getJsonValue() const { return jvalue; }
	operator JsonValue &() { return jvalue; }
};

namespace FieldSchema {

	class Field {
		template<class TClass> friend class TCtor;

	protected:
		intptr_t address;
		string name;

	public:
		virtual ~Field() = default;

		template<class t> bool is() {
			return static_cast<bool>(dynamic_cast<FieldValue<t> *>(this));
		}
		template<class t> t *get(const intptr_t &obj) {
			return is<t>() ? dynamic_cast<FieldValue<t> *>(this)->get(obj) : nullptr;
		}

		const string &getName() const { return name; }
		const intptr_t &getAddress() const { return address; }

		virtual SerialStream stream(const intptr_t &obj) = 0;
		virtual void stream(const intptr_t &obj, SerialStream &stream) = 0;
	};

	template<class TType> class FieldValue : public Field {
	public:
		TType *get(const intptr_t &obj) {
			return reinterpret_cast<TType *>(obj);
		}

		SerialStream stream(const intptr_t &obj) override {
			return JsonValue(string(reinterpret_cast<char *>(get(obj)), sizeof(TType)));
		}
		void stream(const intptr_t &obj, SerialStream &stream) override {
			std::memcpy(get(obj), stream.getJsonValue().to_str().c_str(), sizeof(TType));
		}
	};

	class FieldObjectProxy : public Field {
		template<class TClass> friend class TCtor;

		FieldObject *objectClass;

	public:
		FieldObjectProxy(FieldObject *objectClass) :
			objectClass(objectClass) {
		}
		FieldObject *getMetaObject() {
			return objectClass;
		}

		SerialStream stream(const intptr_t &obj) override { return SerialStream(); }
		void stream(const intptr_t &obj, SerialStream &stream) override {}
	};

	class FieldObject {
		template<class TClass> friend class TCtor;
	
	public:
		class Type {
			template<class TClass> friend class TFieldCtor;
		public:
			virtual ~Type() = default;			
			virtual void *instantiate() = 0;
		};

		template<class TClass> class TType : public Type {
		public:
			void *instantiate() override {
				return new TClass;
			}
		};

	private:
		Type *type;
		Fields fields;
		string name;

	public:
		FieldObject() = default;
		~FieldObject() { delete type; }

		template<class t> bool is() {
			return static_cast<bool>(dynamic_cast<TType<t> *>(type));
		}
		template<class t> t *instantiate() {
			return is<t>() ? reinterpret_cast<t*>(type->instantiate()) : nullptr;
		}
		void *instantiate() {
			return  type->instantiate();
		}

		Fields &getFields() { return fields; }
		const string &getName() const { return name; }
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
	list<string> getNames();
	MetaValue operator[](const string name);
	bool operator==(const MetaObject &other);


	template<class t> bool is() {
		return object->is<t>();
	}
	template<class t> t *get() {
		return is<t>() ? reinterpret_cast<t *>(address) : nullptr;
	}
	operator bool() {
		return address != 0 && object;
	}
	const string &getName() const { return object->getName(); }

	SerialStream stream();
	void stream(SerialStream stream);
};

class MetaValue {
	intptr_t address;
	FieldSchema::Field *field;

public:
	MetaValue(intptr_t address, FieldSchema::Field *field) : address(address), field(field) {}

	bool isObject();
	MetaObject asObject();

	template<class t> bool is() {
		return field->is<t>();
	}
	template<class t> t *get() {
		return field->get<t>(address);
	}

	operator bool() const {
		return address != 0 && field;
	}

	const string &getName() const { return field->getName(); }

	SerialStream stream();
	void stream(SerialStream stream);
};

class MetaClass {
public:
	virtual ~MetaClass() = default;

	MetaObject metaObject();
	virtual const string className() = 0;
};

#include "ActivatorProperties.hpp"
#endif //_activator