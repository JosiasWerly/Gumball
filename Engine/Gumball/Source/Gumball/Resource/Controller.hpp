#pragma once
#ifndef __RESOURCE_CONTROLLER
#define __RESOURCE_CONTROLLER

#include <Resource/Factory.hpp>
#include <Resource/Asset.hpp>
#include <Framework/Archive.hpp>

#include <list>
#include <unordered_map>
#include <string>

namespace Resource {

class GENGINE Controller {
	friend class AssetFactory;

	std::list<FileSerializer *> serializers;
	std::unordered_map<u64, Asset *> assets;

public:
	Controller();
	~Controller();
	
	void LoadFile(const string &filePath);
	void LoadFolder(const string &folderPath);
	void Unload();

	void AddSerializer(FileSerializer *serializer);
	void PopSerializer(const string &extension);
	FileSerializer *GetSerializer(const string &extension);

	void AddAsset(Asset *asset);
	void PopAsset(u64 hash);
	void PopAsset(const string &name);
	Asset *GetAsset(u64 hash);
	Asset *GetAsset(const string &name);
	
	template<class Q> 
	Q *GetContent(const string &name) {
		if (Resource::Asset *asset = GetAsset(name)) {
			if (Ptr<Q> p = asset->Content()) {
				return &(*p);
			}
		}
		return nullptr;
	}
};

};
#endif // !__RESOURCE


//bool AssetModule::Load() {
//	return false;
//
//	loadFolder(Domain::instance()->getContentPath());
//	return true;
//}