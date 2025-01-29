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
	for (auto a : assets)
		delete a;
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
	if (!getAsset(asset->name))
		assets.push_back(asset);
}
void AssetModule::delAsset(Asset *asset) {
	assets.remove(asset);
}
Asset *AssetModule::getAsset(string name) {
	for (auto a : assets) {
		if (a->getName() == name)
			return a;
	}
	return nullptr;
}

void AssetModule::loadFile(const string &filePath) {
	string assetName = Path::fileName(filePath);
	string assetExt = Path::extention(filePath);

	if (getAsset(assetName))
		return;//already loaded

	AssetBuilder *builder = getBuilder(assetExt);
	if (!builder)
		return;//no builder

	Asset *newAsset = new Asset;
	newAsset->name = assetName;
	newAsset->filePath = filePath;

	Archive ar(filePath);
	const bool loaded = builder->load(ar, newAsset->content);
	
	if(loaded)
		addAsset(newAsset);
	else
		delete newAsset;
	
	cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << endl;
}
void AssetModule::loadFolder(const string &folderPath) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(folderPath), end; i != end; ++i) {
		if (!is_directory(i->path()))
			loadFile(i->path().string());
	}
}