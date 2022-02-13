#include <GumballCore/ProjectEntryPoint.hpp>


#include <iostream>
using namespace std;
Extern DLL void OnProjectInit(ProjectLinker &project) {
	cout << "loaded" << endl;
}