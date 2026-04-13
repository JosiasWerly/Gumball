#include "Controller.hpp"
#include "Factory.hpp"
#include <filesystem>

using namespace std;
using namespace Resource;

Controller::Controller() {}
Controller::~Controller() { Unload(); }
void Controller::LoadFile(const string &filePath) {
	throw;
}
void Controller::LoadFolder(const string &folderPath){
	namespace fs = std::filesystem;
	AssetFactory factory;
	for (fs::recursive_directory_iterator i(folderPath), end; i != end; ++i) {
		if (is_directory(i->path()))
			continue;
		factory.Entry(FilePath(i->path().string()));
	}
}
void Controller::Unload(){
	for (auto kv : assets)
		delete kv.second;
	for (auto s : serializers)
		delete s;
}
void Controller::AddSerializer(FileSerializer *serializer){
	serializers.push_back(serializer);
}
void Controller::PopSerializer(const string &extension){
	if (FileSerializer *s = GetSerializer(extension)) {
		serializers.remove(s);
		delete s;
	}
}
FileSerializer *Controller::GetSerializer(const string &extension){
	for (auto &b : serializers) {
		if (b->supports(extension))
			return b;
	}
	return nullptr;
}
void Controller::AddAsset(Asset *asset){
	const u64 hash = asset->Path().hash();
	if (!GetAsset(hash))
		assets[hash] = asset;
}
void Controller::PopAsset(u64 hash){
	if (assets.contains(hash)) {
		delete assets[hash];
		assets.erase(hash);
	}
}
void Controller::PopAsset(const string &name) {
	if (Asset *a = GetAsset(name))
		PopAsset(a->Path().hash());
}
Asset *Controller::GetAsset(u64 hash){
	return assets.contains(hash) ? assets[hash] : nullptr;
}
Asset *Controller::GetAsset(const string &name) {
	for (auto kv : assets) {
		if (kv.second->Path().name() == name)
			return kv.second;
	}
	return nullptr;
}