#include "AssetManager.hpp"
#include "FunctionLibrary.hpp"

#include <iostream>
#include <filesystem>

#include "Engine.hpp"
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
	if (auto a = getAsset(name))
		a->content.free();
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
			Object *content = nullptr;
			if (assembleObject(content, assetPath)) {
				asset->setContent(content);
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
bool AssetsSystem::assembleObject(Object *&content, const string &assetPath) {
	string assetName = Files::getNameOfFilePath(assetPath);
	string assetExt = Files::getExtOfFilePath(assetPath);
	if (auto factory = findFactory(assetExt)) {		
		Archive ar(assetPath);		
		return factory->assemble(content, ar);
	}
	return false;
}


Object* Asset::getContentCloned() {
	Object *obj = nullptr;
	Engine::instance()->assetSystem->assembleObject(obj, this->filePath);
	return obj;
}