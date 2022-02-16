#include "ProjectLinker.hpp"
#include <filesystem>
#include <string>
#include <chrono>


namespace fs = std::filesystem;
using namespace std::chrono_literals;

void ProjectLinker::load() {
	const string localPath = enginePath + "\\target.dll";
	if (dll.isLoaded()) {
		onDettached();
		dll.unload();
	}
	std::filesystem::copy(dllPath, localPath, std::filesystem::copy_options::overwrite_existing);
	//	std::filesystem::copy(dllPath, localPath, std::filesystem::copy_options::recursive);
	if (dll.load(localPath)) {
		onAttached = dll.getFunc<FnxOnProjectAttached>("OnProjectAttached");
		onDettached = dll.getFunc<FnxOnProjectDettached>("OnProjectDettached");

		onAttached(*this);
	}
	else {
		cout << "fail to load" << endl;
	}
}
bool ProjectLinker::hasToLoad() {
	if (dllPath != "") {
		if (!dll.isLoaded())
			return true;

		fs::path p = dllPath;
		ifstream  probeDLL(dllPath);
		if (probeDLL.is_open()) {
			probeDLL.close();
			const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(p));
			const auto curModifiedTime = std::chrono::system_clock::to_time_t(systemTime);
			if (fileModifiedTime != curModifiedTime) {
				fileModifiedTime = curModifiedTime;
				return true;
			}
		}
	}
	return false;
}
