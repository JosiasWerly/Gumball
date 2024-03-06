#include "AssetSystem.hpp"

#include "Engine.hpp"
#include "FunctionLibrary.hpp"
#include "EnviromentVariables.hpp"

#include <iostream>
#include <filesystem>


#include "Shaders.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

namespace Path {
	string extention(string path) {
		return std::filesystem::path(path).extension().string().substr(1);
	}
	string fileName(string path) {
		string fName = std::filesystem::path(path).filename().string();
		return fName.substr(0, fName.find_last_of("."));
	}
};


IAssetFactory *AssetsSystem::findFactory(const string &extension) {
	for (auto &f : factories) {
		if (f->hasExtension(extension))
			return f;
	}
	return nullptr;
}
void AssetsSystem::initialize() {
	factories = {
		new AssetFactory<Image>,
		new AssetFactory<MeshData>,
		new AssetFactory<Shader>
	};
	loadFromFolder(Enviroment::instance()->getResourcePath());
}
void AssetsSystem::shutdown() {
	for (auto a : assets)
		delete a;
}
Asset *AssetsSystem::getAsset(string name) {
	for (auto a : assets) {
		if (a->getName() == name)
			return a;
	}
	return nullptr;
}
void AssetsSystem::addAsset(Asset *asset) {
	if (!getAsset(asset->name))
		assets.push_back(asset);
}
void AssetsSystem::delAsset(Asset *asset) {
	assets.remove(asset);
}
bool AssetsSystem::save(Asset *asset) {
	return false;
}
bool AssetsSystem::load(Asset *asset) {
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
void AssetsSystem::loadFromFolder(string root) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(root), end; i != end; ++i) {
		if (!is_directory(i->path())) {
			loadAssetFromFile(i->path().string());
		}
	}
}
void AssetsSystem::loadAssetFromFile(const string &assetPath) {
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