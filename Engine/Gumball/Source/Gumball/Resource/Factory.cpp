#include "Factory.hpp"
#include "Controller.hpp"
#include <Json/json.hpp>
#include <Gumball/Core/Engine.hpp>
#include <iostream>

using namespace Resource;

void AssetFactory::Entry(FilePath fp) {
	Controller &rs = Controller::Instance();
	if (rs.GetAsset(fp.name()))
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
	
	if (FileSerializer *fs = rs.GetSerializer(newAsset->type)) {
		fs->load(fp, newAsset->content);
		rs.AddAsset(newAsset);
	}

	std::cout << newAsset->filepath.name() << "." << newAsset->filepath.extension() << " --- " << (newAsset->IsValid() ? "ok" : "fail") << std::endl;
}