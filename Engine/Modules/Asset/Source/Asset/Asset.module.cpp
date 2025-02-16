#include "Asset.module.hpp"
#include <Gumball\Domain.hpp>
#include <Gumball\Engine.hpp>

#include <iostream>
#include <filesystem>

using namespace std;

AssetModule::AssetModule() {
}
AssetModule::~AssetModule() {
	for (auto b : builders)
		delete b;
	unload();
}
void AssetModule::posLoad() {
	loadFolder(Domain::instance()->getContentPath());
}
void AssetModule::unload() {
	for (auto kv : assets)
		delete kv.second;
}
void AssetModule::addBuilder(AssetBuilder *builder) {
	builders.push_back(builder);
}
AssetBuilder *AssetModule::getBuilder(const string &extension) {
	for (auto &b : builders) {
		if (b->hasExtension(extension))
			return b;
	}
	return nullptr;
}

void AssetModule::addAsset(Asset *asset) {	
	const u64 hash = asset->getPath().hash();
	if (!getAsset(hash))
		assets[hash] = asset;
}
void AssetModule::delAsset(Asset *asset) {
	const u64 hash = asset->getPath().hash();
	if (assets.contains(hash)) {		
		assets.erase(hash);
	}
}
Asset *AssetModule::getAsset(string name) {
	for (auto kv : assets) {
		if (kv.second->getPath().name() == name)
			return kv.second;
	}
	return nullptr;
}
Asset *AssetModule::getAsset(u64 hash) {
	return assets.contains(hash) ? assets[hash] : nullptr;
}

void AssetModule::loadFile(const string &filePath) {
}
void AssetModule::loadFolder(const string &folderPath) {
	list<FilePath> glfiles, dtfiles;
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(folderPath), end; i != end; ++i) {
		if (!is_directory(i->path())) {
			FilePath fp(i->path().string());
			if (getAsset(fp.hash()))
				continue;//already loaded

			if (fp.extension() == "gfl")
				glfiles.push_back(fp);
			else
				dtfiles.push_back(fp);
		}
	}

	DataAssetFactory dtf;
	dtf.run(dtfiles);

	GLFAssetFactory glf;
	glf.run(glfiles);
}

//void AssetModule::loadFile(const string &filePath) {
//	FilePath fp(filePath);
//	string assetName = fp.name();
//	string assetExt = fp.extension();
//
//	if (getAsset(assetName))
//		return;//already loaded
//
//	AssetBuilder *builder = getBuilder(assetExt);
//	if (!builder)
//		return;//no builder
//
//	Asset *newAsset = new Asset;
//	newAsset->name = assetName;
//	newAsset->filePath = filePath;
//
//	Archive ar(filePath);
//	const bool loaded = builder->load(ar, newAsset->content);
//	
//	if(loaded)
//		addAsset(newAsset);
//	else
//		delete newAsset;
//	
//	cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << endl;
//}
//void AssetModule::loadFolder(const string &folderPath) {
//	list<FilePath> indexer;
//	namespace fs = std::filesystem;
//	for (fs::recursive_directory_iterator i(folderPath), end; i != end; ++i) {
//		if (!is_directory(i->path())) {
//			FilePath fp(i->path().string());
//			if (fp.extension() == "glf") {
//				indexer.push_back(i->path().string());
//			}
//			else {
//				loadDataAsset(fp);
//			}
//		}
//	}
//
//	for (auto it = indexer.begin(); it != indexer.end(); ++it) {
//	}
//}
//void AssetModule::loadDataAsset(const FilePath &fp) {
//	AssetBuilder *builder = getBuilder(fp.extension());
//	if (!builder)
//		return;//no builder
//
//	Asset *newAsset = new Asset;
//	newAsset->name = fp.name();
//	newAsset->filePath = fp.path();
//
//	Archive ar(fp.path());
//	const bool loaded = builder->load(ar, newAsset->content);
//
//	if (loaded)
//		addAsset(newAsset);
//	else
//		delete newAsset;
//}