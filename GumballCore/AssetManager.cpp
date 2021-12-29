#include "AssetManager.hpp"
#include "FunctionLibrary.hpp"

#include <iostream>
#include <filesystem>


#include "Shaders.hpp"
#include "Texture.hpp"

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
	factories.push_back(new TextureFactory);
}
void AssetsSystem::shutdown() {
}
void AssetsSystem::loadFile(const string &assetPath) {
	string assetName = Files::getNameOfFilePath(assetPath);
	if (!this->operator[](assetName)) {
		if (auto factory = findFactory(assetPath)) {
			Asset *asset = new Asset(assetName);
			Archive ar(assetPath);
			if (factory->assemble(*asset, ar)) {
				assets.push_back(asset);
			}
			else {				
				delete asset;
				asset = nullptr;
			}
			cout << assetPath << " --- " << (asset ? "ok" : "fail") << endl;
		}
	}
}
void AssetsSystem::loadAllFiles(string root) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(root), end; i != end; ++i) {
		if (!is_directory(i->path()))
			loadFile(i->path().string());
	}
}
IAssetFactory *AssetsSystem::findFactory(const string &ext) {
	for (auto i : factories) {
		if (i->probeFile(ext))
			return i;
	}
	return nullptr;
}
