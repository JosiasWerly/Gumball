#include "AssetFactory.hpp"
#include "Asset.module.hpp"

#include <iostream>
#include "Json/json.hpp"

void AssetFactory::entry(FilePath fp) {
	AssetModule *am = AssetModule::instance();
	if (am->getAsset(fp.name()))
		return;//loaded
	
	Asset *newAsset = new Asset();
	newAsset->filepath = fp;
	if (fp.extension() == "gfl") {
		Json json = Json::parse(Archive(fp.path()).read<string>());
		newAsset->type = json["type"];
		auto jArr = json["dependencies"].array();
		for (Json::iterator it = jArr.begin(); it != jArr.end(); ++it) {
			newAsset->dependencies.push_back((string)*it);
		}
	}
	else {
		newAsset->type = fp.extension();
		newAsset->dependencies = {};
	}
	am->getSerializer(newAsset->type)->load(fp, newAsset->content);
	std::cout << newAsset->filepath.name() << "." << newAsset->filepath.extension() << " --- " << (newAsset->isValid() ? "ok" : "fail") << std::endl;
	am->addAsset(newAsset);
}
void AssetFactory::flush() {
}
//bool DataAsset::load(FilePath &fp) {
//	AssetModule *am = AssetModule::instance();
//
//	const string assetName = fp.name();
//	const string assetExt = fp.extension();
//
//	if (am->getAsset(assetName))
//		continue;//already loaded
//
//	AssetBuilder *builder = am->getBuilder(assetExt);
//	if (!builder)
//		continue;//no builder
//
//	Archive ar(fp.path());
//	Asset *newAsset = new Asset(fp.extension(), fp);
//	const bool loaded = builder->load(ar, newAsset->getContent());
//	if (loaded)
//		am->addAsset(newAsset);
//	else
//		delete newAsset;
//	std::cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << std::endl;
//}
	
	//AssetModule *am = AssetModule::instance();
	//for (auto &fp : files) {
	//	const string assetName = fp.name();
	//	const string assetExt = fp.extension();

	//	if (am->getAsset(assetName))
	//		continue;//already loaded

	//	AssetBuilder *builder = am->getBuilder(assetExt);
	//	if (!builder)
	//		continue;//no builder

	//	Archive ar(fp.path());
	//	Asset *newAsset = new Asset(fp.extension(), fp);
	//	const bool loaded = builder->load(ar, newAsset->getContent());
	//	if (loaded)
	//		am->addAsset(newAsset);
	//	else
	//		delete newAsset;
	//	std::cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << std::endl;
	//}

//void GLFAssetFactory::run(FilePath &fp) {
//	AssetModule *am = AssetModule::instance();
//	for (auto &fp : files) {
//		const string assetName = fp.name();
//
//		if (am->getAsset(assetName))
//			continue;//already loaded
//
//		Archive ar(fp.path());
//		if (!ar.isOpen())
//			continue;//cannot open file
//
//		Json json = Json::parse(ar.read<string>());
//		
//		if (!json.contains("type"))
//			continue;//no type
//		string type = json["type"];
//		
//		AssetBuilder *builder = am->getBuilder(type);
//		if (!builder)
//			continue;//no builder
//
//		if (!json.contains("dependencies") || !json["dependencies"].is_array())
//			continue;
//		list<FilePath> dependencies;
//		auto jArr = json["dependencies"].array();
//		for (Json::iterator it = jArr.begin(); it != jArr.end(); ++it) {
//			dependencies.push_back((string)*it);
//		}
//		
//
//		if (!json.contains("content"))
//			continue;
//		string content = json[content];
//
//		Asset *newAsset = new Asset(type, fp, dependencies);
//		const bool loaded = builder->load(ar, newAsset->getContent());
//		if (loaded)
//			am->addAsset(newAsset);
//		std::cout << assetName << "." << newAsset->getPath().extension() << " --- " << (loaded ? "ok" : "fail") << std::endl;
//	}
//}