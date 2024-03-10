#include "Activator.hpp"
#include "picojson.h"
#include "Math.hpp"
using namespace std;


map<string, string> fromJson(const string &json) {
    map<string, string> out;

	picojson::value jvalue;	
	string err = picojson::parse(jvalue, json);
	if (!err.empty()) {
		return out;
	}

	if (jvalue.is<picojson::object>()) {
		picojson::object jobj = jvalue.get<picojson::object>();
		for (auto it : jobj) {
			out[it.first] = it.second.to_str();
		}
	}
    return out;
}
string toJson(map<string, string> props) {
	picojson::object jobj;
	for (const auto &pair : props) {
		jobj[pair.first] = picojson::value(pair.second);
	}
	return picojson::value(jobj).serialize();
}

void Activator::add(ClassType builder) {
	signatures.insert(builder);
}
ClassType *Activator::get(const string name) {
	auto it = std::find_if(
		signatures.begin(), signatures.end(),
		[=](const ClassType &i) { return i.getName() == name; }
	);
	return it == signatures.end() ? nullptr : const_cast<ClassType*>(&(*it));
}
Class *Activator::load(const string &data) {
	auto jdata = fromJson(data);
	
	ClassType *classType = get(jdata["class"]);
	if (!classType) {
		return nullptr;
	}

	Class *obj = classType->getNew();
	if (!obj) {
		return nullptr;
	}

	auto &properties = classType->getProperties();
	for (auto kv : jdata) {
		if (properties.contains(kv.first)) {
			properties[kv.first]->load(obj, kv.second);
		}
	}
	return obj;
}
string Activator::save(Class *obj) { 
	ClassType *classType = obj->getType();
	if (!classType) {
		return "nullptr";
	}
	
	map<string, string> jdata;
	jdata["class"] = classType->getName();

	auto &properties = classType->getProperties();
	for (auto kv : properties) {
		jdata[kv.first] = kv.second->save(obj);
	}
	return toJson(jdata);
}


#include <iostream>
using namespace std;

class Super : public Class {
public:
	Super() {
		cout << "Super ";
	}

	virtual ClassType *getType() { return Activator::instance()->get("Super"); }
};
class A : public Super {
public:
	A() {
		cout << "A" << endl;
	}
	int a, b, c;
	string d;

	virtual ClassType *getType() { return Activator::instance()->get("A"); }
};
class B : public Super {
public:
	B() {
		cout << "B" << endl;
	}

	virtual ClassType *getType() { return Activator::instance()->get("B"); }
};
class C : public Super {
public:
	C() {
		cout << "C" << endl;
	}
};

void test() {
	auto activator = Activator::instance();

	activator->add(
		ClassTypeBuilder<Super>().signature("Super")
	);
	activator->add(
		ClassTypeBuilder<A>().signature("A")
		.properties()
		.add<int>("a", &A::a)
		.add<int>("b", &A::b)
		.add<int>("c", &A::c)
	);
	activator->add(ClassTypeBuilder<B>().signature("B")
	);

	{//save, load
		A a;
		a.a = 0xABCDDCBA;
		a.b = 0xFAFAAFAF;
		a.c = 0xCADAADCA;
		string data = activator->save(&a);
		Class *ca = activator->load(data);

		cout << "test";
	}
}


//Class *Activator::packageFind(Package &package, string name) {
//	auto it = std::find_if( package.begin(), package.end(),
//		[=](const Class *i) {
//			return i->getName() == name; 
//		}
//	);
//	return it != package.end() ? *it : nullptr;
//}
//void Activator::add(string packageName, ActivatorPackage package) {
//	packages[packageName] = package.signatures;
//}
//void Activator::del(string packageName) {
//	if (!packages.contains(packageName)) {
//		return;
//	}
//
//	Package &package = packages[packageName];
//	for (auto i : package) {
//		delete i;
//	}
//	packages.erase(packageName);
//}
//Class *Activator::get(string name) {
//	for (auto kv : packages) {
//		if (Class *probe = packageFind(kv.second, name)) {
//			return probe;
//		}
//	}
//	return nullptr;
//}
//Class *Activator::getAt(string packageName, string name) {
//	if (packages.contains(packageName)) {
//		return packageFind(packages[packageName], name);
//	}
//	return nullptr;
//}