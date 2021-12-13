#include "AssetManager.hpp"
#include "FunctionLibrary.hpp"

#include <iostream>
#include <filesystem>


#include "Shaders.hpp"

bool IAssetFactory::probeFile(const string &filePath) {
	string extension = Files::getExtOfFilePath(filePath);
	for (auto &ex : extensions)	{
		if (ex == extension)
			return true;
	}
	return false;
}

void AssetsSystem::initialize() {
	factories.push_back(new ShaderFactory);
}
void AssetsSystem::shutdown() {
}
void AssetsSystem::loadAsset(const string &assetPath) {
	Asset *asset = new Asset();
	if (auto factory = findFactory(assetPath)) {
		Archive ar;
		ar.open(assetPath);
		if (factory->assemble(*asset, ar))
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
		if (i->probeFile(ext))
			return i;
	}
	return nullptr;
}
