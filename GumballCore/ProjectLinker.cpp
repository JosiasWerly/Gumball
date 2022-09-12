#include "ProjectLinker.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;
using namespace std::chrono_literals;



void ProjectLinker::setup(string dllPath, string enginePath) {
	this->dllPath = dllPath;	
	this->enginePath = enginePath.substr(0, enginePath.find_last_of("\\"));
}
IProject* ProjectLinker::linkerTargetInstance() {
	const string localPath = enginePath + "\\target.dll";
	if (dll.isLoaded())
		dll.unload();


	std::filesystem::copy(dllPath, localPath, std::filesystem::copy_options::overwrite_existing);
	if (dll.load(localPath)) {
		auto fnxAttchment = dll.getFunc<FnxOnProjectAttached>("OnProjectAttached");
		if (!fnxAttchment)
			return nullptr;

		IProject *outProject = fnxAttchment();
		if (!outProject)
			return nullptr;

		{
			fs::path p = dllPath;
			const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
			const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
			fileModifiedTime = curModifiedTime;
		}
		return outProject;
	}	
	return nullptr;
}
bool ProjectLinker::isNewLinkerAvailable() {
	if (dllPath != "") {
		if (!dll.isLoaded())
			return true;

		fs::path p = dllPath;
		ifstream probeDLL(dllPath);
		if (probeDLL.is_open()) {
			probeDLL.close();
			{
				const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
				const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
				if (fileModifiedTime != curModifiedTime)
					return true;
			}
		}
	}
	return false;
}




//void ProjectLinker::load() {
//	const string localPath = enginePath + "\\target.dll";
//	if (dll.isLoaded()) {
//		onDettached();
//		dll.unload();
//	}
//	std::filesystem::copy(dllPath, localPath, std::filesystem::copy_options::overwrite_existing);
//	//	std::filesystem::copy(dllPath, localPath, std::filesystem::copy_options::recursive);
//	if (dll.load(localPath)) {
//		if (FnxOnProjectAttached projAttach = dll.getFunc<FnxOnProjectAttached>("OnProjectAttached")) {
//			if (projectBind = projAttach());
//
//			{
//				fs::path p = dllPath;
//				const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
//				const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
//				fileModifiedTime = curModifiedTime;
//			}
//		}
//	}
//	else {
//		cout << "fail to load" << endl;
//	}
//}