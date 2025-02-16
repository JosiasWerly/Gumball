#include "AssetFactory.hpp"
#include "Asset.module.hpp"

#include <iostream>
#include "Json/picojson.h"

void DataAssetFactory::run(list<FilePath> &files) {
	AssetModule *am = AssetModule::instance();
	for (auto &fp : files) {
		const string assetName = fp.name();
		const string assetExt = fp.extension();

		if (am->getAsset(assetName))
			return;//already loaded

		AssetBuilder *builder = am->getBuilder(assetExt);
		if (!builder)
			return;//no builder

		Archive ar(fp.path());
		Asset *newAsset = new Asset(fp);
		const bool loaded = builder->load(ar, newAsset->getContent());
		if (loaded)
			am->addAsset(newAsset);
		else
			delete newAsset;
		std::cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << std::endl;
	}
}

void GLFAssetFactory::run(list<FilePath> &files) {
	typedef picojson::object JsonObject;
	typedef picojson::value JsonValue;

	AssetModule *am = AssetModule::instance();
	for (auto &fp : files) {
		const string assetName = fp.name();
		const string assetExt = fp.extension();

		if (am->getAsset(assetName))
			return;//already loaded

		Archive ar(fp.path());
		string arContent;
		ar >> arContent;

		JsonValue jval;
		picojson::parse(jval, arContent);


		const picojson::value::object &obj = jval.get<picojson::object>();
		for (picojson::value::object::const_iterator i = obj.begin(); i != obj.end(); ++i) {
			std::cout << i->first << ': ' << i->second.to_str() << std::endl;
		}

		//Archive ar(fp.path());
		//
		//if (am->getAsset(assetName))
		//	return;//already loaded
		//
		//AssetBuilder *builder = am->getBuilder(assetExt);
		//if (!builder)
		//	return;//no builder
		//
		//Asset *newAsset = new Asset(fp);
		//const bool loaded = builder->load(ar, newAsset->getContent());
		//if (loaded)
		//	am->addAsset(newAsset);
		//else
		//	delete newAsset;
		//std::cout << assetName << "." << assetExt << " --- " << (loaded ? "ok" : "fail") << std::endl;
	}
}