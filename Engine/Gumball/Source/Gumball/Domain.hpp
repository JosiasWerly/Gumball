#pragma once
#ifndef _domain
#define _domain

#include "Patterns.hpp"
#include <string>
using namespace std;

class GENGINE Domain : public Singleton<Domain> {
	friend class Engine;

	string applicationPath;
	string applicationDir;
	string contentPath;

	Domain() = default;
public:
	const string &getApplicationPath() { return applicationPath; }
	const string &getApplicationDir() { return applicationDir; }
	const string &getContentPath() { return contentPath; }
};
#endif // !_domain