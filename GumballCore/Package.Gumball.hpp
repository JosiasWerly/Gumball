#pragma once
#ifndef _packagegumball
#define _packagegumball

#include "Activator.hpp"

struct ClassTypePackage_Core : public ClassTypePackage {
	string name() { return "core"; }
	std::list<ClassType *> types();
};

#endif //_packagegumball