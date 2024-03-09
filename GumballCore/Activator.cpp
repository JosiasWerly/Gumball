#include "Activator.hpp"

TypeWrapper *Activator::packageFind(Package &package, const char *name) {
	return *std::find_if(
		package.begin(), package.end(),
		[=](const TypeWrapper *i) {
			return i->getName() == name; 
		}
	);
}
void Activator::add(const char *packageName, ActivatorPackage package) {
	packages[packageName] = package.signatures;
}
void Activator::del(const char *packageName) {
	if (!packages.contains(packageName)) {
		return;
	}

	Package &package = packages[packageName];
	for (auto i : package) {
		delete i;
	}
	packages.erase(packageName);
}
TypeWrapper *Activator::get(const char *name) {
	for (auto kv : packages) {
		if (TypeWrapper *probe = packageFind(kv.second, name)) {
			return probe;
		}
	}
	return nullptr;
}
TypeWrapper *Activator::getAt(const char *packageName, const char *name) {
	if (packages.contains(packageName)) {
		return packageFind(packages[packageName], name);
	}
	return nullptr;
}