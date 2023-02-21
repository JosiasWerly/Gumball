#pragma once
#ifndef _object
#define _object
#include "Var.hpp"
#include "Archive.hpp"
class GBCORE Object {
public:
	virtual ~Object() {}

	virtual Var<Object> clone() const { throw "NotImplemented"; return nullptr; }
	virtual bool archiveLoad(Archive &ar) { throw "NotImplemented"; return false; }
	virtual bool archiveSave(Archive &ar) { throw "NotImplemented"; return false; }
};

#endif // !_object