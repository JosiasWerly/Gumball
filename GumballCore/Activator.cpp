#include "Activator.hpp"
#include "picojson.h"
using namespace std;

void Activator::add(FieldSchema::FieldObject *object) {
	if (!object)
		return;

	if (objects.contains(object->getName()))
		return;

	objects[object->getName()] = object;
}
void Activator::del(const string &name) {
	if (!objects.contains(name))
		return;

	delete objects[name];
	objects.erase(name);
}
FieldSchema::FieldObject *Activator::get(const string &name) {
	return objects.contains(name) ? objects[name] : nullptr;
}


void *SerialStream::toMetaClass() {
	JsonObject jsonObject = getJsonValue().get<picojson::object>();

	if (!jsonObject.contains("class")) {
		return nullptr;
	}

	FieldSchema::FieldObject *fieldObject = Activator::instance()->get(jsonObject["class"].to_str());
	if (!fieldObject) {
		return nullptr;
	}

	return fieldObject->instantiate();
}
string SerialStream::toString(bool pretty) const { 
	return jvalue.serialize(pretty); 
}


bool MetaValue::isObject() {
	return static_cast<bool>(dynamic_cast<FieldSchema::FieldObjectProxy *>(field));
}
MetaObject MetaValue::asObject() {
	return isObject() ? MetaObject(address, dynamic_cast<FieldSchema::FieldObjectProxy *>(field)->getMetaObject()) : MetaObject();
}
SerialStream MetaValue::stream() {
	return isObject() ? asObject().stream() : field->stream(address);
}
void MetaValue::stream(SerialStream stream) {
	return isObject() ? asObject().stream(stream) : field->stream(address, stream);
}


list<MetaValue> MetaObject::getValues() {
	FieldSchema::Fields &fields = object->getFields();
	list<MetaValue> out;
	for (auto kv : fields) {
		FieldSchema::Field *field = kv.second;
		MetaValue newProp(address + field->getAddress(), field);
		out.push_back(newProp);
	}
	return out;
}
list<string> MetaObject::getNames() { return {}; }
MetaValue MetaObject::operator[](const string name) { return MetaValue(0, 0); }
bool MetaObject::operator==(const MetaObject &other) { return false; }
SerialStream MetaObject::stream() {
	picojson::object jsonObject;
	picojson::object jsonValues;

	auto properties = getValues();
	for (auto p : properties) {
		jsonValues[p.getName()] = p.stream();
	}

	jsonObject["class"] = picojson::value(object->getName());
	jsonObject["fields"] = picojson::value(jsonValues);
	return SerialStream(picojson::value(jsonObject));
}
void MetaObject::stream(SerialStream stream) {
	JsonObject jsonObject = stream.getJsonValue().get<picojson::object>();

	if (object->getName() != jsonObject["class"].to_str()) {
		return;
	}

	JsonObject jsonValues = jsonObject["fields"].get<picojson::object>();
	auto properties = getValues();
	for (auto p : properties) {
		if (jsonValues.contains(p.getName())) {
			p.stream(jsonValues[p.getName()]);
		}
	}
}


MetaObject MetaClass::metaObject(){
	return MetaObject(reinterpret_cast<intptr_t>(this), Activator::instance()->get(metaName()));
}