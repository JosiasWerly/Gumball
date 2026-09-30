#pragma once
#ifndef __domain
#define __domain

#include <Gumball/Core/Engine.hpp>
#include <string>
namespace Core {
class Engine;

class GENGINE Domain : public Engine::Global<Domain> {
	friend class Engine;
	std::string applicationPath;
	std::string applicationDir;
	std::string engineDir;
	std::string contentPath;

public:
	Domain() = default;
	const std::string &ApplicationPath() const { return applicationPath; }
	const std::string &ApplicationDir() const { return applicationDir; }
	const std::string &EngineDir() const { return engineDir; }
	const std::string &ContentPath() const { return contentPath; }
};

};
#endif // !__domain