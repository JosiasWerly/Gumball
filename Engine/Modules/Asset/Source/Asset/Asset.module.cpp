#include "Asset.module.hpp"
#include <Gumball\Domain.hpp>
#include <Gumball\Engine.hpp>

#include <iostream>
#include <filesystem>

using namespace std;

struct Test {

};
template<>
class GMODULE WFileSerializer<Test> : public TFileSerializer<Test> {
public:
	bool load(const FilePath &fp, Test &val) {
		return true;
	}
	bool save(const FilePath &fp, Test &val) {
		return true;
	}
	bool supports(const FilePath &fp) const {
		return fp.path() == "fun";
	}
};

AssetModule::AssetModule() {
	addSerializer(new WFileSerializer<Test>);
}
AssetModule::~AssetModule() {
	for (auto s : serializers)
		delete s;
	unload();
}
void AssetModule::posLoad() {
	loadFolder(Domain::instance()->getContentPath());
}
void AssetModule::unload() {
	for (auto kv : assets)
		delete kv.second;
}
void AssetModule::addSerializer(FileSerializer *builder) {
	serializers.push_back(builder);
}
FileSerializer *AssetModule::getSerializer(const string &extension) {
	for (auto &b : serializers) {
		if (b->supports(extension))
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
	namespace fs = std::filesystem;
	AssetFactory factory;
	for (fs::recursive_directory_iterator i(folderPath), end; i != end; ++i) {
		if (is_directory(i->path()))
			continue;
		factory.entry(FilePath(i->path().string()));
	}
	factory.flush();
}