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


bool MetaValue::isObject() {
	return static_cast<bool>(dynamic_cast<FieldSchema::FieldObjectProxy *>(field));
}
MetaObject MetaValue::asObject() {
	return isObject() ? MetaObject(address, dynamic_cast<FieldSchema::FieldObjectProxy *>(field)->getMetaObject()) : MetaObject();
}
SerialStream MetaValue::toStream() {
	return isObject() ? asObject().toStream() : field->toStream(address);
}
void MetaValue::fromStream(SerialStream stream) {
	return isObject() ? asObject().fromStream(stream) : field->fromStream(address, stream);
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
SerialStream MetaObject::toStream() {
	picojson::object jsonObject;
	picojson::object jfields;

	auto properties = getValues();
	for (auto p : properties) {
		jfields[p.getName()] = p.toStream();
	}

	jsonObject["class"] = picojson::value(object->getName());
	jsonObject["fields"] = picojson::value(jfields);
	return SerialStream(picojson::value(jsonObject));
}
void MetaObject::fromStream(SerialStream stream) {
	JsonObject jobj = stream.getJsonValue().get<picojson::object>();

	if (object->getName() != jobj["class"].to_str()) {
		return;
	}

	JsonObject jfields = jobj["fields"].get<picojson::object>();
	auto properties = getValues();
	for (auto p : properties) {
		if (jfields.contains(p.getName())) {
			p.fromStream(jfields[p.getName()]);
		}
	}
}


MetaObject MetaClass::getMetaObject(){
	return MetaObject(reinterpret_cast<intptr_t>(this), Activator::instance()->get(getClassName()));
}