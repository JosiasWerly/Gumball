#pragma once
#ifndef _functionlibrary
#define _functionlibrary

#include "Patterns.hpp"
#include <string>
using namespace std;

namespace Files {
	string getExtOfFilePath(string path);
	string getNameOfFilePath(string path);
};
#endif // !_functionlibrary