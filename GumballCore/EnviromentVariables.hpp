#pragma once
#ifndef _enviromentvariables
#define _enviromentvariables

#include "Patterns.hpp"
#include <string>
using namespace std;

class GBCORE Enviroment :
	public Singleton<Enviroment> {
	friend class Engine;

	string applicationPath;
	string applicationDir;
	string resourcePath;
	string contentPath;

	Enviroment() {}
public:
	const string &getApplicationPath() { return applicationPath; }
	const string &getApplicationDir() { return applicationDir; }
	const string &getResourcePath() { return resourcePath; }
	const string &getContentPath() { return contentPath; }
};
#endif // !_enviromentvariables