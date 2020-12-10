#include "AssetManager.hpp"
#include <iostream>
#include <filesystem>
using namespace std;
void AssetManager::loadAssets(string rootFolderPath) {
	namespace fs = std::filesystem;
	for (fs::recursive_directory_iterator i(rootFolderPath), end; i != end; ++i)
		if (!is_directory(i->path())) {
			this->loadAssetFromDisk(i->path().string());
		}
}