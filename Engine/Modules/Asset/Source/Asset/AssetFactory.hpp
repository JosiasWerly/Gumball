#ifndef __assetfactory
#define __assetfactory

#include <Gumball/Archive.hpp>
#include "Asset.hpp"

class GMODULE AssetFactory {
public:
	void entry(FilePath fp);
	void flush();
};
#endif // __assetfactory