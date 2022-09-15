#include "ProjectLinker.hpp"
#include "EnviromentVariables.hpp"


#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;
using namespace std::chrono_literals;


void ProjectLinker::hotReload() {
	if (dll.isLoaded())
		dll.unload();

	auto e = Enviroment::instance();
	const string dllPath = e->getApplicationDir() + "GumballProject.dll";
	const string dllCopy = e->getApplicationDir() + "Target.dll";

	std::filesystem::copy(dllPath, dllCopy, std::filesystem::copy_options::overwrite_existing);
	if (dll.load(dllCopy)) {
		auto projectEntryPoint = dll.getFunc<FnxEntryPoint>("EntryPoint");
		if (!projectEntryPoint)
			return;

		Project *outProject = projectEntryPoint();
		if (!outProject)
			return;
		{
			fs::path p = dllPath;
			const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
			const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
			fileModifiedTime = curModifiedTime;
		}
		project = outProject;
	}
}
void ProjectLinker::unload() {
	delete project;
	project = nullptr;
	if (dll.isLoaded())
		dll.unload();
}
Project *ProjectLinker::getProject() {
	return project;
}
bool ProjectLinker::isProjectLoaded() {
	return project;
}
bool ProjectLinker::hasNewVersion() {
	if (!dll.isLoaded())
		return true;

	const string dllPath = Enviroment::instance()->getApplicationDir() + "GumballProject.dll";
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

	return false;
}



#ifdef GBCORE
Extern GBCORE void GumballCoreEntryPoint() {

}
#endif // GBCore
