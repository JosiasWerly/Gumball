#ifndef _asset
#define _asset

#include <Gumball/Archive.hpp>
#include <list>
#include <set>
using namespace std;

class GMODULE Asset {
	friend class AssetFactory;
private:
	FilePath filepath;
	string type;
	list<FilePath> dependencies;
	PtrWeak content;

public:
	Inline FilePath &getPath() { return filepath; }
	Inline list<FilePath> &getDependencies() { return dependencies; }
	Inline PtrWeak & getContent() { return content; }
	Inline bool isLoaded() const { return content; }
	Inline bool isValid() const { return filepath && content; }
};

#endif // !_asset