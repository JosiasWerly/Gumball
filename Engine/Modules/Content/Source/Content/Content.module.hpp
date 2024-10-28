#pragma once
#ifndef __assetmodule
#define __assetmodule

#include "Asset.hpp"

#include <list>
#include <set>
#include <string>

class GMODULE ContentModule : public ModuleSingleton<ContentModule> {
private:
	std::list<BaseAssetFactory *> factories;
	std::list<Asset *> assets;

	BaseAssetFactory *findFactory(const string &extension);

protected:
	void posLoad() override;
	void unload() override;
	EModuleTickType tickType() const override { return EModuleTickType::none; }
	const char* name() const override { return "Content"; }
public:

	ContentModule();
	void addFactory(BaseAssetFactory *factory);
	void delFactory(BaseAssetFactory *factory);
	void addAsset(Asset *asset);
	void delAsset(Asset *asset);
	Asset *getAsset(string name);
	template<class t> t *getContent(string name) {
		if (auto asset = getAsset(name)) {
			if (auto var = dynamic_cast<Var<t>*>(asset->getContent())) {
				return var->pin();
			}
		}
		return nullptr;
	}

	bool save(Asset *asset);
	bool load(Asset *asset);

	void loadFromFolder(string root);
	void loadAssetFromFile(const string &assetPath);
};

#endif // !__assetmodule