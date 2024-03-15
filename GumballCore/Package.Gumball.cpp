#include "Package.Gumball.hpp"

#include "WorldSystem.hpp"
#include "Actor.hpp"



class Super : public ImplClass {
public:
	const string getClassName() { return "Super"; }

	Super() {
		cout << "Super ";
	}
	double q, w;
};
class A : public ImplClass {
public:
	const string getClassName() { return "A"; }

	A() {
		cout << "A" << endl;
	}
	float fa, fb;
};
class B : public ImplClass {
public:
	const string getClassName() { return "B"; }

	B() {
		cout << "B" << endl;
	}
	int a, b, c;
	string str;
	A obj0;
	Super obj1;
};
class C : public ImplClass {
public:
	const string getClassName() { return "C"; }

	C() {
		cout << "C" << endl;
	}
	int *pa;
};
class D : public C {
public:
	const string getClassName() { return "D"; }

	D() {
		cout << "D" << endl;
	}
	A objA;
};


void test() {


	auto activator = Activator::instance();

	activator->add(
		TFieldCtor<Super>("Super")
		.prop<double>("q", &Super::q)
		.prop<double>("w", &Super::w)
	);

	activator->add(
		TFieldCtor<A>("A")
		.prop<float>("q", &A::fa)
		.prop<float>("w", &A::fb)
	);

	activator->add(
		TFieldCtor<B>("B")
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


	B t0;
	t0.getClass().fromStream(b.getClass().toStream());	
	MetaObject t0Class = t0.getClass();

	{
		std::ofstream outFile("output.json");
		if (outFile.is_open()) {
			outFile << t0Class.toStream().toString();
			outFile.close();
		}
	}

	{
		cout << endl;
		cout << t0Class.getName() << " cast ";
		if (t0Class.is<A>()) {
			cout << (t0Class.as<A>() ? "true" : "false") << endl;
		}

		if (t0Class.is<B>()) {
			cout << (t0Class.as<B>() ? "true" : "false") << endl;
		}

		if (t0Class.is<Super>()) {
			cout << (t0Class.as<Super>() ? "true" : "false") << endl;
		}
	}

	{
		cout << endl;
		auto properties = t0Class.getFields();
		for (auto &it : properties) {
			if (it.isObject()) {
				if (MetaObject subclass = it.asObject()) {
					cout << "########" << "class " << it.getName() << "########" << endl;
					for (auto &subit : subclass.getFields()) {
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

						if (int *ptr = subit.as<int>()) {
							cout << *ptr;
						}

						if (float *ptr = subit.as<float>()) {
							cout << *ptr;
						}

						if (double *ptr = subit.as<double>()) {
							cout << *ptr;
						}

						if (string *ptr = subit.as<string>()) {
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

				if (int *ptr = it.as<int>()) {
					cout << *ptr;
				}

				if (float *ptr = it.as<float>()) {
					cout << *ptr;
				}

				if (double *ptr = it.as<double>()) {
					cout << *ptr;
				}

				if (string *ptr = it.as<string>()) {
					cout << *ptr;
				}
				cout << endl;
			}
		}

		cout << endl << endl;
	}
}
