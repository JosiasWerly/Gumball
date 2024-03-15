#include "Activator.hpp"
#include "picojson.h"
using namespace std;



bool MetaField::isObject() {
	return static_cast<bool>(dynamic_cast<FieldObjectProxy *>(field));
}
MetaObject MetaField::asObject() {
	return isObject() ? MetaObject(address, dynamic_cast<FieldObjectProxy *>(field)->getMetaObject()) : MetaObject();
}
SerialStream MetaField::toStream() {
	return isObject() ? asObject().toStream() : field->toStream(address);
}
void MetaField::fromStream(SerialStream stream) {
	return isObject() ? asObject().fromStream(stream) : field->fromStream(address, stream);
}

list<MetaField> MetaObject::getFields() {
	Fields &fields = object->getFields();
	list<MetaField> out;
	for (auto kv : fields) {
		Field *field = kv.second;
		MetaField newProp(address + field->getAddress(), field);
		out.push_back(newProp);
	}
	return out;
}

SerialStream MetaObject::toStream() {
	picojson::object jsonObject;
	picojson::object jfields;

	auto properties = getFields();
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
	auto properties = getFields();
	for (auto p : properties) {
		if (jfields.contains(p.getName())) {
			p.fromStream(jfields[p.getName()]);
		}
	}
}




void Activator::add(FieldObject *object) {
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
FieldObject *Activator::get(const string &name) {
	return objects.contains(name) ? objects[name] : nullptr;
}

MetaObject ImplClass::getClass(){
	return MetaObject(reinterpret_cast<intptr_t>(this), Activator::instance()->get(getClassName()));
}