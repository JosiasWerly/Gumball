#include <GumballCore/ProjectLinker.hpp>
#include <iostream>
using namespace std;



class Calculator {
	typedef float (*Fnx)(float a, float b);
	map<string, Fnx> opr = {
		{
			"+",
			[](float a, float b)->float {
				return a + b; 
			}
		},
		{
			"*",
			[](float a, float b)->float {
				return a * b;
			}
		},
		{
			"-",
			[](float a, float b)->float {
				return a - b;
			}
		},
		{
			"/",
			[](float a, float b)->float {
				return a / b;
			}
		}
	};


public:
	Calculator() {
	}
	~Calculator() {
	}
	void proc() {
		cout << opr["/"](8, 4) << endl;
	}
};
Extern void OnProjectAttached(ProjectLinker &project) {
	cout << "<< loaded" << endl;
	Calculator c;
	c.proc();
}
Extern void OnProjectDettached() {
	cout << ">> unloaded" << endl;
}