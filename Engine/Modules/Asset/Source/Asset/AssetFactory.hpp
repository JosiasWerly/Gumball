#ifndef __assetfactory
#define __assetfactory

#include <Gumball/Archive.hpp>

class GMODULE AssetFactory {
public:
	virtual void run(list<FilePath> &files) = 0;
};


class GMODULE DataAssetFactory : public AssetFactory {
public:
	void run(list<FilePath> &files);
};

class GMODULE GLFAssetFactory : public AssetFactory {
public:
	void run(list<FilePath> &files);
};

#endif // __assetfactory