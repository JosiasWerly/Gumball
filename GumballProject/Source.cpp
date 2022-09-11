#include <iostream>
#include <map>
#include <string>
using namespace std;


#include <GumballCore/ProjectLinker.hpp>
#include <GumballCore/SceneOverlay.hpp>
#include <GumballCore/WidgetOverlay.hpp>

Extern void OnProjectAttached(Engine &engineRef) {
	cout << "<< loaded >> " << endl;
}
Extern void OnProjectDettached() {
	cout << ">> unloaded" << endl;
}