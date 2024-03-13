#include "Activator.hpp"
#include "picojson.h"
using namespace std;


void Activator::add(ClassType *classType) {
	if (!classType)
		return;

	if (classTypes.contains(classType->getName()))
		return;

	classTypes[classType->getName()] = classType;
}
void Activator::del(const string &name) {
	if (!classTypes.contains(name))
		return;

	if (!classTypes.contains(name))
		return;

	delete classTypes[name];
	classTypes.erase(name);
}
ClassType *Activator::get(const string className) {
	return classTypes.contains(className) ? classTypes[className] : nullptr;
}
Class *Activator::load(const string &data) {
	//picojson::object jsonObject;
	//{
	//	picojson::value jsonValue;
	//	string err = picojson::parse(jsonValue, data);
	//	if (!err.empty() || !jsonValue.is<picojson::object>()) {
	//		return nullptr;
	//	}
	//	jsonObject = jsonValue.get<picojson::object>();
	//}
	//
	//ClassType *classType = get(jsonObject["class"].to_str());
	//if (!classType) {
	//	return nullptr;
	//}

	//Class *obj = classType->getNew();
	//if (!obj) {
	//	return nullptr;
	//}

	//auto &properties = classType->getProperties();
	//picojson::object props = jsonObject["props"].get<picojson::object>();
	//for (auto kv : props) {
	//	if (properties.contains(kv.first)) {
	//		properties[kv.first]->serialize(obj, kv.second.to_str());
	//	}
	//}
	////jsonObject["data"] TODO
	//return obj;
	return nullptr;
}
string Activator::save(Class *obj) { 
	/*ClassType *classType = obj->getClassType();
	if (!classType) {
		return "nullptr";
	}
	
	picojson::object jsonObject;
	jsonObject["class"] = picojson::value(classType->getName());


	picojson::object props;
	for (auto kv : classType->getProperties()) {
		props[kv.first] = picojson::value(kv.second->serialize(obj));
	}	
	jsonObject["props"] = picojson::value(props);


	jsonObject["data"] = picojson::value("todo");
	return picojson::value(jsonObject).serialize(false);*/

	return "";
}