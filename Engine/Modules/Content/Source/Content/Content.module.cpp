#include "Content.module.hpp"
#include <Gumball\Domain.hpp>
#include <Gumball\Engine.hpp>

#include <iostream>
#include <filesystem>

using namespace std;

ContentModule::ContentModule() {
}
BaseAssetFactory *ContentModule::findFactory(const string &extension) {
	for (auto &f : factories) {
		if (f->hasExtension(extension))
			return f;
	}
	return nullptr;
}
void ContentModule::posLoad() {
	loadFromFolder(Domain::instance()->getContentPath());
}
void ContentModule::unload() {
	for (auto a : assets)
		delete a;
}
void ContentModule::addFactory(BaseAssetFactory *factory) {
	factories.push_back(factory);
}
void ContentModule::delFactory(BaseAssetFactory *factory) {
	factories.remove(factory);
}
void ContentModule::addAsset(Asset *asset) {
	if (!getAsset(asset->name))
		assets.push_back(asset);
}
void ContentModule::delAsset(Asset *asset) {
	assets.remove(asset);
}
Asset *ContentModule::getAsset(string name) {
	for (auto a : assets) {
		if (a->getName() == name)
			return a;
	}
	return nullptr;
}
bool ContentModule::save(Asset *asset) {
	return false;
}
bool ContentModule::load(Asset *asset) {
	Archive ar(asset->filePath);
	if (ar.isOpen()) {
		string assetExt = Path::extention(asset->filePath);
		if (auto factory = findFactory(assetExt)) {
			if (factory->archiveLoad(ar, asset->content)) {
				return true;
			}
		}
	}
	return false;
}
void ContentModule::loadFromFolder(string root) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(root), end; i != end; ++i) {
		if (!is_directory(i->path())) {
			loadAssetFromFile(i->path().string());
		}
	}
}
void ContentModule::loadAssetFromFile(const string &assetPath) {
	bool loaded = false;
	string assetName = Path::fileName(assetPath);
	string assetExt = Path::extention(assetPath);
	if (!getAsset(assetName)) {
		Asset *newAsset = new Asset;
		newAsset->name = assetName;
		newAsset->filePath = assetPath;
		if (loaded = load(newAsset))
			addAsset(newAsset);
		else
			delete newAsset;
		cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << endl;
	}
}