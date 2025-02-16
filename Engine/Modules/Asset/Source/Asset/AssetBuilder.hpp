#ifndef __assetbuilder
#define __assetbuilder

#include <Gumball/Archive.hpp>

class GMODULE AssetBuilder {
public:
	virtual bool load(Archive &ar, Ptr &p) = 0;
	virtual bool save(Archive &ar, Ptr &p) = 0;
	virtual bool hasExtension(const string &extention) const = 0;
};

template<class T>
class TAssetBuilder : public AssetBuilder {
	virtual bool load(Archive &ar, T &p) = 0;
	virtual bool save(Archive &ar, T &p) = 0;
	virtual bool hasExtension(const string &extention) const = 0;

	bool load(Archive &ar, Ptr &p) {
		TPtr newPtr(new T);
		if (!load(ar, *newPtr))
			~newPtr;
		p = newPtr.ptr();
		return newPtr;
	}
	bool save(Archive &ar, Ptr &p) {
		if (!p)
			return false;
		return save(ar, *p.ref<T>());
	}
};

template<class T>
class WAssetBuilder : public TAssetBuilder<T> {};

#endif // !__assetbuilder