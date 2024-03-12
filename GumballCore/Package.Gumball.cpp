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
	A obj;

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

std::list<ClassType *> ClassTypePackage_Core::types() {
	return {
		ClassTypeCtor<Super>("Super"),

		ClassTypeCtor<A>("A"),

		ClassTypeCtor<B>("B")
		.prop<int>("a", &B::a)
		.prop<int>("b", &B::b)
		.prop<int>("c", &B::c)
		.prop<string>("str", &B::str)
		.prop<A>("obj", &B::obj),

		//ClassTypeCtor<C>("B")
		//.prop<int *>("pa", &C::pa),
	};
}

void test() {
	auto activator = Activator::instance();

	B bObj;
	bObj.a = 0xABCDDCBA;
	bObj.b = 0xFAFAAFAF;
	bObj.c = 0xCADAADCA;
	bObj.str = "mystring";
	bObj.obj.fa = 1.123f;
	bObj.obj.fb = -4.321f;

	string data = activator->save(&bObj);

	bObj.a = 0xAAABBB;
	bObj.b = 0xCCCDDD;
	bObj.c = 0xEEEFFF;
	bObj.str = "superfun";
	bObj.obj.fa = 333.333f;
	bObj.obj.fb = -111.111f;


	Class *ca = activator->load(data);
	ClassType *caType = ca->getClassType();
	
	Properties properties = caType->getProperties();
	for (auto &kv : properties) {
		string name = kv.first;
		Property *prop = kv.second;

		if (prop->is<int>()) {
			cout << "int " << name << " " << *prop->as<int>(ca) << endl;
		}
		if (string *casted = prop->as<string>(ca)) {
			cout << "string " << name << " " << *casted << endl;
		}
	}
	return;
}