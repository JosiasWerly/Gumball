#pragma once
#ifndef __assetmodule
#define __assetmodule

#include "Asset.hpp"

#include <list>
#include <set>
#include <string>

class GMODULE AssetModule : public ModuleSingleton<AssetModule> {
private:
	std::list<AssetBuilder *> builders;
	std::list<Asset *> assets;


protected:
	void posLoad() override;
	void unload() override;
	EModuleTickType tickType() const override { return EModuleTickType::none; }
	const char* name() const override { return "Content"; }

public:
	AssetModule();
	~AssetModule();
	void addBuilder(AssetBuilder *builder);
	AssetBuilder *getBuilder(const string &extension);
	
	void addAsset(Asset *asset);
	void delAsset(Asset *asset);
	Asset *getAsset(string name);
	
	template<class t> 
	t *getContent(string name) {
		if (auto asset = getAsset(name)) {
			if (TPtr<t> p = asset->content.ptr<t>()) {
				return &(*p);
			}
		}
		return nullptr;
	}

	void loadFile(const string &filePath);
	void loadFolder(const string &folderPath);
};

#endif // !__assetmodule