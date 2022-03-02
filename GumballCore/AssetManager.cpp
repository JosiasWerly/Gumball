#include "AssetManager.hpp"
#include "FunctionLibrary.hpp"

#include <iostream>
#include <filesystem>


#include "Shaders.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

bool IAssetFactory::hasExtension(const string &extention) {
	for (auto &ex : extensions) {
		if (ex == extention)
			return true;
	}
	return false;
}

void AssetsSystem::initialize() {
	factories = {
		new ShaderFactory,
		new TextureFactory,
		new MeshFactory,
	};
}
void AssetsSystem::shutdown() {
}
Asset *AssetsSystem::getAsset(string name) {
	for (auto a : assets) {
		if (a->getName() == name)
			return a;
	}
	return nullptr;
}
Asset *AssetsSystem::createAsset(string name) {
	Asset *newAsset = getAsset(name);
	if (!newAsset) {
		newAsset = new Asset;
		newAsset->name = name;
		assets.push_back(newAsset);
	}
	return newAsset;
}
void AssetsSystem::unloadAsset(string name) {
	if (auto a = getAsset(name)) {
		delete *a->content;
	}
}
void AssetsSystem::loadAssetFromFile(const string &assetPath) {
	bool loaded = false;
	string assetName = Files::getNameOfFilePath(assetPath);
	string assetExt = Files::getExtOfFilePath(assetPath);
	if (!getAsset(assetName)) {
		if (auto factory = findFactory(assetExt)) {
			Asset *asset = new Asset;
			asset->name = assetName;
			asset->filePath = assetPath;
			Archive ar(assetPath);
			if (factory->assemble(*asset, ar)) {
				assets.push_back(asset);
				loaded = true;
			}
			else {
				delete asset;
				asset = nullptr;
			}
		}
		cout << assetPath << " --- " << (loaded ? "ok" : "fail") << endl;
	}
}
void AssetsSystem::loadAssetsFromFolder(string root) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(root), end; i != end; ++i) {
		if (!is_directory(i->path()))
			loadAssetFromFile(i->path().string());
	}
}
void AssetsSystem::reloadAsset(string name) {

}
IAssetFactory *AssetsSystem::findFactory(const string &extension) {
	for (auto f : factories) {
		if (f->hasExtension(extension))
			return f;
	}
	return nullptr;
}
void AssetsSystem::createFactory(IAssetFactory *newFactory) {
	factories.push_back(newFactory);
}
