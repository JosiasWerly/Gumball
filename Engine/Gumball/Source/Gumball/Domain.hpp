#pragma once
#ifndef __domain
#define __domain

#include "Engine.hpp"
#include "Singleton.hpp"
#include <string>

class GENGINE Domain : public EngineSingleton<Domain> {
	friend class Engine;
	std::string applicationPath;
	std::string applicationDir;
	std::string contentPath;

	Domain() = default;
public:
	const std::string &getApplicationPath() const { return applicationPath; }
	const std::string &getApplicationDir() const { return applicationDir; }
	const std::string &getContentPath() const { return contentPath; }
};
#endif // !__domain