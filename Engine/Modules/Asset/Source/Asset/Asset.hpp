#ifndef _asset
#define _asset

#include <Gumball/Archive.hpp>
#include <list>
#include <set>
using namespace std;

class GMODULE Asset {
private:
	FilePath filepath;
	string type;
	list<FilePath> dependencies;
	Ptr content;

public:
	Asset(FilePath filepath, list<FilePath> dependencies = {}) :
		filepath(filepath),
		dependencies(dependencies)
	{}
	Inline FilePath &getPath() { return filepath; }
	Inline list<FilePath> &getDependencies() { return dependencies; }
	Inline Ptr& getContent() { return content; }
	Inline bool isLoaded() const { return content; }
	Inline bool isValid() const { return filepath && content; }
};

#endif // !_asset