#pragma once
#ifndef __assetmodule
#define __assetmodule

#include "AssetFactory.hpp"
#include "Asset.hpp"

#include <list>
#include <set>
#include <string>

class GMODULE AssetModule : public Plugin::Singleton<AssetModule> {
	friend class AssetFactory;

private:
	std::list<FileSerializer *> serializers;
	std::unordered_map<u64, Asset *> assets;

protected:
	bool Load() override;
	void Unload() override;
	Plugin::eTick TickType() const override { return Plugin::eTick::none; }
	const char* Name() const override { return "Content"; }

public:
	AssetModule();
	~AssetModule();
	void addSerializer(FileSerializer *builder);
	FileSerializer *getSerializer(const string &extension);
	
	void addAsset(Asset *asset);
	void delAsset(Asset *asset);
	Asset *getAsset(string name);
	Asset *getAsset(u64 hash);
	
	template<class t> 
	t *getContent(string name) {
		if (auto asset = getAsset(name)) {
			if (Ptr<t> p = asset->getContent()) {
				return &(*p);
			}
		}
		return nullptr;
	}

	void loadFile(const string &filePath);
	void loadFolder(const string &folderPath);
};

#endif // !__assetmodule