#include "Project.hpp"
#include <Gumball/Core/Domain.hpp>


#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

using namespace std;
using namespace Plugin;
namespace fs = std::filesystem;
using namespace std::chrono_literals;


Project *ProjectLinker::Load() {
	//auto d = Domain::instance();
	//const string dllPath = d->getApplicationDir() + "Sandbox.dll";
	//const string dllCopy = d->getApplicationDir() + "Target.dll";
	//
	//std::filesystem::copy(dllPath, dllCopy, std::filesystem::copy_options::overwrite_existing);
	//if (dll.load(dllCopy)) {
	//	auto projectEntryPoint = dll.getFunc<FnxEntryPoint>("EntryPoint");
	//	if (!projectEntryPoint)
	//		return nullptr;
	//
	//	project = projectEntryPoint();
	//	if (!project)
	//		return nullptr;
	//	
	//	{
	//		fs::path p = dllPath;
	//		const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
	//		const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
	//		fileModifiedTime = curModifiedTime;
	//	}
	//	project->attached();
	//	return project;
	//}
	return nullptr;
}
void ProjectLinker::Unload() {
	if (project) {
		project->Detached();
		delete project;
	}
	project = nullptr;
	if (dll.isLoaded())
		dll.unload();
}
bool ProjectLinker::HasNewVersion() {
	if (!dll.isLoaded())
		return true;

	//const string dllPath = Domain::instance()->getApplicationDir() + "GumballProject.dll";
	//fs::path p = dllPath;
	//ifstream probeDLL(dllPath);
	//if (probeDLL.is_open()) {
	//	probeDLL.close();
	//	{
	//		const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
	//		const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
	//		if (fileModifiedTime != curModifiedTime)
	//			return true;
	//	}
	//}
	return false;
}

#ifdef GENGINE
Extern GENGINE void GumballCoreEntryPoint() {

}
#endif