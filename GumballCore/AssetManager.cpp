#include "AssetManager.hpp"
#include "FunctionLibrary.hpp"

#include <iostream>
#include <filesystem>


bool IAssetFactory::canBuild(const string &filePath) {
	string extension = Files::getExtOfFilePath(filePath);
	for (auto &ex : extensions)	{
		if (ex == extension)
			return true;
	}
	return false;
}

void AssetsSystem::initialize() {

}
void AssetsSystem::shutdown() {

}
void AssetsSystem::loadAsset(const string &assetPath) {
	Asset *asset = new Asset();
	if (auto factory = findFactory(assetPath)) {
		if (factory->load(assetPath, asset))
			assets.push_back(asset);
		else
			delete asset;
	}
}
void AssetsSystem::loadAllAssets(string root) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(root), end; i != end; ++i) {
		if (!is_directory(i->path()))
			loadAsset(i->path().string());
	}
}
IAssetFactory *AssetsSystem::findFactory(const string &ext) {
	for (auto i : factories) {
		if (i->canBuild(ext))
			return i;
	}
	return nullptr;
}
