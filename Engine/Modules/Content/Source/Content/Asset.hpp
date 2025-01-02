#ifndef _asset
#define _asset

#include <Gumball/Archive.hpp>
#include <Gumball/Var.hpp>

#include <list>
#include <set>
using namespace std;


class GMODULE Asset {
	friend class ContentModule;

private:
	string filePath = "";
	string name = "";

protected:
	Var *content = nullptr;

public:
	Inline Var *getContent() { return content; }
	Inline bool isValid() { return content != nullptr; }
	Inline const string &getName() { return name; }
	Inline const string &getPath() { return filePath; }
};

class GMODULE BaseAssetFactory {
protected:
	set<string> extensions;

public:
	BaseAssetFactory() {}
	bool hasExtension(const string &extention) const { return extensions.contains(extention); }
	virtual bool archiveLoad(Archive &ar, Var *&var) = 0;
	virtual bool archiveSave(Archive &ar, const Var *var) = 0;
};

template<class T> class GMODULE TAssetFactory : public BaseAssetFactory {
public:
	bool archiveLoad(Archive &ar, Var *&var) override final {
		delete var;
		T *trg = new T;
		if (load(ar, *trg)) {
			var = new TVar<T>(trg);
			return true;
		}
		delete trg;
		return false;
	}
	bool archiveSave(Archive &ar, const Var *var) override final {
		throw;
		return false;
	}

	virtual bool load(Archive &ar, T &val) = 0;
	virtual bool save(Archive &ar, const T &val) = 0;
};

template<class T> class GMODULE AssetFactory : public TAssetFactory<T> {
};
#endif // !_asset