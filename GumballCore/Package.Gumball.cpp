#include "Package.Gumball.hpp"

#include "WorldSystem.hpp"
#include "Actor.hpp"



class Super : public Class {
public:
	Super() {
		cout << "Super ";
	}

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

template<class T> 
class Foo {
public:
	int ImConcrete;
};


void test() {
	//C cObj;
	//TProperty<C, int*> prop(&C::pa);
	


	auto activator = Activator::instance();

	B bObj;
	bObj.a = 0xABCDDCBA;
	bObj.b = 0xFAFAAFAF;
	bObj.c = 0xCADAADCA;
	bObj.str = "mystring";
	bObj.obj.fa = 1.123f;
	bObj.obj.fb = -4.321f;

	string data = activator->save(&bObj);
	Class *ca = activator->load(data);

	return;
}