#include "Package.Gumball.hpp"

#include "WorldSystem.hpp"
#include "Actor.hpp"



class Super : public Class {
public:
	Super() {
		cout << "Super ";
	}
	int q, w;
	virtual ClassType *getClassType() { return Activator::instance()->get("Super"); }
};
class A : public Super {
public:
	A() {
		cout << "A" << endl;
	}

	float fa, fb;
	virtual ClassType *getClassType() { return Activator::instance()->get("A"); }
};
class B : public Super {
public:
	B() {
		cout << "B" << endl;
	}

	int a, b, c;
	string str;
	A obj0;
	Super obj1;

	virtual ClassType *getClassType() { return Activator::instance()->get("B"); }
};
class C : public Super {
public:
	C() {
		cout << "C" << endl;
	}

	int *pa;
	virtual ClassType *getClassType() { return Activator::instance()->get("C"); }
};

class D : public C {
public:
	D() {
		cout << "D" << endl;
	}

	A objA;
	virtual ClassType *getClassType() { return Activator::instance()->get("D"); }
};

//std::list<ClassType *> ClassTypePackage_Core::types() {
//	return {
//		ClassTypeCtor<Super>("Super"),
//
//		ClassTypeCtor<A>("A"),
//
//		ClassTypeCtor<B>("B")
//		.prop<int>("a", &B::a)
//		.prop<int>("b", &B::b)
//		.prop<int>("c", &B::c)
//		.prop<string>("str", &B::str)
//		.prop<A>("obj", &B::obj),
//
//		//ClassTypeCtor<C>("B")
//		//.prop<int *>("pa", &C::pa),
//	};
//}

void test() {
	auto activator = Activator::instance();

	activator->add(
		ClassCtor<Super>("Super")
		.prop<int>("q", &Super::q)
		.prop<int>("w", &Super::w)
	);
	activator->add(
		ClassCtor<A>("A")
		.prop<float>("q", &A::fa)
		.prop<float>("w", &A::fb)
	);

	activator->add(
		ClassCtor<B>("B")
		.prop<int>("a", &B::a)
		.prop<A>("A", "obj0", &B::obj0)
		.prop<Super>("Super", "obj1", &B::obj1)
		.prop<int>("b", &B::b)
		.prop<int>("c", &B::c)
		.prop<string>("str", &B::str)
	);

	B bObj;
	bObj.a = 0xABCDDCBA;
	bObj.b = 0xFAFAAFAF;
	bObj.c = 0xCADAADCA;
	bObj.str = "mystring";
	bObj.obj0.fa = 1.123f;
	bObj.obj0.fb = -4.321f;
	bObj.obj1.q = 4444;
	bObj.obj1.w = 1111;

	Class *bClass = &bObj;
	ClassType *bClassType = bObj.getClassType();
	Properties &properties = bClassType->getProperties();

	//##### Concrete #####//
	for (auto kv : properties) {
		Property *prop = kv.second;

		if (prop->is<int>()) {
			cout << "int " << prop->getName() << " " << *prop->as<int>(bClass) << endl;
		}
		
		if (string *str = prop->as<string>(bClass)) {
			cout << "str " << prop->getName() << " " << *str << endl;
		}
		
		if (A *a = prop->as<A>(bClass)) {
			cout << "A " << prop->getName() << " " << a->fa << "," << a->fb << endl;
		}

		if (Super *super = prop->as<Super>(bClass)) {
			cout << "Super " << prop->getName() << " " << super->q << "," << super->w << endl;
		}
	}

	//##### Abstract #####//
	for (auto kv : properties) {
		Property *prop = kv.second;
		
		if (prop->is<ClassType>()) {
			cout << "class " << prop->getName() << endl;
		}

		if (auto classType = prop->as<ClassType>()) {
			cout << "class " << prop->getName() << " props: " << classType->getProperties().size() << endl;
		}
	}


	//string data = activator->save(&bObj);
	//
	//bObj.a = 0xAAABBB;
	//bObj.b = 0xCCCDDD;
	//bObj.c = 0xEEEFFF;
	//bObj.str = "superfun";
	//bObj.obj.fa = 333.333f;
	//bObj.obj.fb = -111.111f;


	//Class *classObj = activator->load(data);
	//ClassType *classType = classObj->getClassType();	
	//Properties properties = classType->getProperties();
	//
	//
	////##### Concrete #####//
	//for (auto &kv : properties) {
	//	string name = kv.first;
	//	Property *prop = kv.second;
	//
	//	if (prop->is<int>()) {
	//		cout << "int " << name << " " << *prop->as<int>(classObj) << endl;
	//	}
	//	if (string *casted = prop->as<string>(classObj)) {
	//		cout << "string " << name << " " << *casted << endl;
	//	}
	//}
	//
	////##### Abstraction #####//
	//for (auto &kv : properties) {
	//	string name = kv.first;
	//	Property *prop = kv.second;
	//
	//	if (Property *innerProp = prop->as<Property>(classObj)) {
	//		cout << "prop " << innerProp->
	//	}
	//}
	//return;
}