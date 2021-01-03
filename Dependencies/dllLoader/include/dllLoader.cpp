#include "dllLoader.hpp"

DynamicLibrary::DynamicLibrary(string name, HINSTANCE instance) :
	name(name),
	instance(instance){
}


#pragma warning( push )
#pragma warning( disable : 4244 )
bool DynamicLibraryManager::load(string fullPath, string name) {
	auto WSTRtoSTR = [](wstring& from)->string {
		string out;
		for (size_t i = 0; i < from.length(); i++)
			out += from.at(i);
		return out;
	};
	auto STRtoWSTR = [](string& from)->wstring {
		wstring out;
		for (size_t i = 0; i < from.length(); i++)
			out += from.at(i);
		return out;
	};

	if (this->operator[](name))
		return false;
	
	wstring wFullPath = STRtoWSTR(fullPath);
	HINSTANCE instance = LoadLibrary((LPCWSTR)wFullPath.c_str());
	if (!instance)
		return false;
	
	dynamicLibraries.push_back(new DynamicLibrary(name, instance));
	return true;
}
#pragma warning( pop )
void DynamicLibraryManager::free(string name) {
	DynamicLibrary* dll = this->operator[](name);
	if (dll) {
		dynamicLibraries.remove(dll);
		FreeLibrary((HINSTANCE)dll->instance);
		delete dll;
		dll = nullptr;
	}
}
DynamicLibrary* DynamicLibraryManager::operator[](string name) {
	for (auto& d : dynamicLibraries)
		if (d->name == name)
			return d;
	return nullptr;
}