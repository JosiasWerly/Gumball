#include "Activator.hpp"
#include "picojson.h"
using namespace std;

void Activator::add(ClassType *classType) {
	if (!classType)
		return;

	if (signatures.contains(classType->getName()))
		return;

	signatures[classType->getName()] = classType;
}
void Activator::del(const string &name) {
	if (!signatures.contains(name))
		return;

	if (!signatures.contains(name))
		return;

	delete signatures[name];
	signatures.erase(name);
}
ClassType *Activator::get(const string className) {
	return signatures.contains(className) ? signatures[className] : nullptr;
}
Class *Activator::load(const string &data) {
	picojson::object jsonObject;
	{
		picojson::value jsonValue;
		string err = picojson::parse(jsonValue, data);
		if (!err.empty() || !jsonValue.is<picojson::object>()) {
			return nullptr;
		}
		jsonObject = jsonValue.get<picojson::object>();
	}
	
	ClassType *classType = get(jsonObject["class"].to_str());
	if (!classType) {
		return nullptr;
	}

	Class *obj = classType->getNew();
	if (!obj) {
		return nullptr;
	}

	auto &properties = classType->getProperties();
	picojson::object props = jsonObject["props"].get<picojson::object>();
	for (auto kv : props) {
		if (properties.contains(kv.first)) {
			properties[kv.first]->load(obj, kv.second.to_str());
		}
	}

	//jsonObject["data"] TODO

	return obj;
}
string Activator::save(Class *obj) { 
	ClassType *classType = obj->getType();
	if (!classType) {
		return "nullptr";
	}
	
	picojson::object jsonObject;
	jsonObject["class"] = picojson::value(classType->getName());


	picojson::object props;
	for (auto kv : classType->getProperties()) {
		props[kv.first] = picojson::value(kv.second->save(obj));
	}	
	jsonObject["props"] = picojson::value(props);


	jsonObject["data"] = picojson::value("todo");
	return picojson::value(jsonObject).serialize(false);
}
template<class T> void Activator::addPackage() {
	ClassTypePackage *data = new T;
	const string &packageName = data->name();
	if (!packages.contains(packageName)) {
		packages.emplace(packageName, Package());
	}
	Package &package = packages[packageName];

	std::list<ClassType *> types = data->types();
	for (auto type : types) {
		add(type);
		package.push_back(type->getName());
	}
	delete data;
}
template<class T> void Activator::delPackage() {
	ClassTypePackage *data = new T;
	const string packageName = data->name();
	
	if (!packages.contains(packageName))
		return;

	Package &package = packages[packageName];
	for (auto &name : package) {
		del(name);
	}
	packages.erase(packageName);
	delete data;
}