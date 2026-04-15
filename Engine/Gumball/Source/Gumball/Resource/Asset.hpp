#ifndef _ASSET
#define _ASSET

#include <Gumball/Framework/Archive.hpp>
#include <list>

namespace Resource {

class GENGINE Asset {
	friend class AssetFactory;
private:
	FilePath filepath;
	string type;
	std::list<FilePath> dependencies;
	PtrVoid content;

public:
	Inline FilePath &Path() { return filepath; }
	Inline std::list<FilePath> &Dependencies() { return dependencies; }
	Inline PtrVoid &Content() { return content; }
	Inline bool IsLoaded() const { return content; }
	Inline bool IsValid() const { return filepath && content; }
};

};
#endif // !_ASSET