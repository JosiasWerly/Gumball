#include "AssetManager.hpp"
#include <iostream>
#include <filesystem>

void AssetsSystem::Initialize() {

}
void AssetsSystem::Shutdown() {

}
void AssetsSystem::loadAsset(const string &assetPath) {
	if (auto factory = findFactory(assetPath)) {
		factory->load(assetPath);
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
