#include "Package.Gumball.hpp"

#include "WorldSystem.hpp"
#include "Actor.hpp"



class Super : public MetaClass {
public:
	const string metaName() { return "Super"; }

	Super() {
		cout << "Super ";
	}
	double q, w;
};
class A : public MetaClass {
public:
	const string metaName() { return "A"; }

	A() {
		cout << "A" << endl;
	}
	float fa, fb;
};
class B : public MetaClass {
public:
	const string metaName() { return "B"; }

	B() {
		cout << "B" << endl;
	}
	int a, b, c;
	string str;
	A obj0;
	Super obj1;
};
class C : public MetaClass {
public:
	const string metaName() { return "C"; }

	C() {
		cout << "C" << endl;
	}
	int *pa;
};
class D : public C {
public:
	const string metaName() { return "D"; }

	D() {
		cout << "D" << endl;
	}
	A objA;
};


void test() {


	auto activator = Activator::instance();

	activator->add(
		FieldSchema::TCtor<Super>("Super")
		.prop<double>("q", &Super::q)
		.prop<double>("w", &Super::w)
	);

	activator->add(
		FieldSchema::TCtor<A>("A")
		.prop<float>("q", &A::fa)
		.prop<float>("w", &A::fb)
	);

	activator->add(
		FieldSchema::TCtor<B>("B")
		.prop<int>("a", &B::a)
		.prop<int>("b", &B::b)
		.prop<int>("c", &B::c)
		.prop<string>("str", &B::str)
		.prop<A>("A", "obj0", &B::obj0)
		.prop<Super>("Super", "obj1", &B::obj1)
	);

	
	B b;
	b.a = 11;
	b.b = 22;
	b.c = 33;
	b.str = "mystring";
	b.obj0.fa = 0.123f;
	b.obj0.fb = -321.0f;
	b.obj1.q = 55.55;
	b.obj1.w = 66.66;

	SerialStream stream = b.metaObject().stream();
	
	B *pb = stream.toMetaClass<B>();

	B &t0 = *pb;
	t0.metaObject().stream(b.metaObject().stream());
	MetaObject t0MetaObj = t0.metaObject();

	{
		std::ofstream outFile("output.json");
		if (outFile.is_open()) {
			outFile << t0MetaObj.stream().toString(true);
			outFile.close();
		}
	}

	{
		cout << endl;
		cout << t0MetaObj.getName() << " cast ";
		if (t0MetaObj.is<A>()) {
			cout << (t0MetaObj.get<A>() ? "true" : "false") << endl;
		}

		if (t0MetaObj.is<B>()) {
			cout << (t0MetaObj.get<B>() ? "true" : "false") << endl;
		}

		if (t0MetaObj.is<Super>()) {
			cout << (t0MetaObj.get<Super>() ? "true" : "false") << endl;
		}
	}

	{
		cout << endl;
		auto properties = t0MetaObj.getValues();
		for (auto &it : properties) {
			if (it.isObject()) {
				if (MetaObject subclass = it.asObject()) {
					cout << "########" << "class " << it.getName() << "########" << endl;
					for (auto &subit : subclass.getValues()) {
						if (subit.is<int>()) {
							cout << "int ";
						}

						if (subit.is<float>()) {
							cout << "float ";
						}

						if (subit.is<double>()) {
							cout << "double ";
						}

						if (subit.is<string>()) {
							cout << "string ";
						}

						cout << subit.getName() << " = ";

						if (int *ptr = subit.get<int>()) {
							cout << *ptr;
						}

						if (float *ptr = subit.get<float>()) {
							cout << *ptr;
						}

						if (double *ptr = subit.get<double>()) {
							cout << *ptr;
						}

						if (string *ptr = subit.get<string>()) {
							cout << *ptr;
						}
						
						cout << endl;
					}
					cout << "################" << endl;
				}
			}
			else {
				if (it.is<int>()) {
					cout << "int ";
				}

				if (it.is<float>()) {
					cout << "float ";
				}

				if (it.is<double>()) {
					cout << "double ";
				}

				if (it.is<string>()) {
					cout << "string ";
				}

				cout << it.getName() << " = ";

				if (int *ptr = it.get<int>()) {
					cout << *ptr;
				}

				if (float *ptr = it.get<float>()) {
					cout << *ptr;
				}

				if (double *ptr = it.get<double>()) {
					cout << *ptr;
				}

				if (string *ptr = it.get<string>()) {
					cout << *ptr;
				}
				cout << endl;
			}
		}

		cout << endl << endl;
	}
}
