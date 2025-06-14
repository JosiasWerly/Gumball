#ifndef __pointer
#define __pointer
#include <concepts>

struct PtrWrapper {
	virtual ~PtrWrapper() {}
	virtual operator void *() = 0;
};
template<class T>
struct TPtrWrapper : public PtrWrapper {
	T *ptr = nullptr;
	TPtrWrapper(T *init) : ptr(init) {}
	~TPtrWrapper() { delete ptr; ptr = nullptr; }
	operator void *() { return ptr; }
};
class PtrController {
public:
	unsigned refs = 1;
	PtrWrapper *pw = nullptr;

	template<class U> PtrController(U *init) : pw(new TPtrWrapper<U>(init)) {}
	~PtrController() { delete pw; }
};

template<class T> class TPtr;
template<class T> class TRef;
class Ptr;
class Ref;

template<class T>
class PtrPad {
	template<class t> friend class PtrPad;

protected:
	PtrController *ctrl;

	Inline void iref() { ++ctrl->refs; }
	Inline void dref() { --ctrl->refs; }
	Inline void delCtrl() { delete ctrl; ctrl = nullptr; }
	Inline void delPtr() { delete ctrl->pw; ctrl->pw = nullptr; }

	Inline void assing(PtrController *otherCtrl) { divest(); ctrl = otherCtrl; iref(); }
	Inline void divest() { dref(); if (!ctrl->refs) { delCtrl(); } }
	template<class U> const U *castT() const { return dynamic_cast<const U *>(this->operator->()); }

	PtrPad(PtrController *ctrlInit) : ctrl(ctrlInit) {}

public:
	PtrPad() = delete;
	PtrPad(PtrPad &&) = delete;
	PtrPad(const PtrPad &) = delete;
	PtrPad &operator=(PtrPad &&other) = delete;
	PtrPad &operator=(const PtrPad &other) = delete;
	~PtrPad() = default;

	template<class U>
	bool operator==(const PtrPad<U> &other) const
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		return ctrl == other.ctrl;
	}

	T &operator*() {
		return *static_cast<TPtrWrapper<T>*>(ctrl->pw)->ptr;
	}
	T *const operator->() {
		return static_cast<TPtrWrapper<T>*>(ctrl->pw)->ptr;
	}
	const T &operator*() const {
		return *static_cast<TPtrWrapper<T>*>(ctrl->pw)->ptr;
	}
	const T *const operator->() const {
		return static_cast<TPtrWrapper<T>*>(ctrl->pw)->ptr;
	}
	operator bool() const {
		return ctrl && ctrl->pw && ctrl->pw->operator void *();
	}
	operator void *() const {
		return ctrl ? (*ctrl->pw) : nullptr;
	}

	template<class U> 
	const U *dcast() const { 
		return dynamic_cast<const U *>(this->operator->()); 
	}

	template<class U>
	const U *scast() const {
		return static_cast<const U *>(this->operator->());
	}

	template<class U>
	TPtr<U> ptr()
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		if (!castT<U>())
			throw;
		return TPtr<U>(*ctrl);
	}

	template<class U>
	TRef<U> ref()
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		if (!castT<U>())
			throw;
		return TRef<U>(*ctrl);
	}

	auto ptr();
	auto ref();

};

template<class T>
class TPtr : public PtrPad<T> {
	template<class t> friend class PtrPad;
	template<class t> friend class TPtr;
	friend class Ptr;
	friend class Ref;
	typedef PtrPad<T> Super;

	TPtr(PtrController &ctrl) : Super(&ctrl) { Super::iref(); }

public:
	TPtr(T *pInit = nullptr) : Super(new PtrController(pInit)) {}
	TPtr(const TPtr &init) : Super(init.ctrl) { Super::iref(); }
	~TPtr() { Super::divest(); }

	void operator~() {
		Super::delPtr();
	}
	TPtr &operator=(const TPtr &other) {
		Super::assing(other.ctrl);
		return *this;
	}

	template<class U>
	const TPtr<T> &operator=(const TPtr<U> &other)
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		if (!other.castT<T>())
			throw;
		Super::assing(other.ctrl);
		return *this;
	}

	template<class U>
	const TPtr<T> &operator=(TPtr<U> &&other)
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		if (!other.castT<T>())
			throw;
		Super::assing(other.ctrl);
		return *this;
	}
};

template<class T>
class TRef : public PtrPad<T> {
	template<class t> friend class PtrPad;
	template<class t> friend class TRef;
	friend class Ptr;
	friend class Ref;
	typedef PtrPad<T> Super;

	TRef(PtrController &ctrl) : Super(&ctrl) {}

public:
	TRef() : Super(nullptr) {}
	TRef(const TRef &init) : Super(init.ctrl) {}
	~TRef() = default;

	TRef &operator=(const TRef &other) {
		Super::assing(other.ctrl);
		return *this;
	}

	template<class U>
	const TRef<T> &operator=(const TRef<U> &other)
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		if (!other.castT<T>())
			throw;
		Super::ctrl = other.ctrl;
		return *this;
	}

	template<class U>
	const TRef<T> &operator=(TRef<U> &&other)
		requires std::convertible_to<T, U> || std::convertible_to<U, T> {
		if (!other.castT<T>())
			throw;
		Super::ctrl = other.ctrl;
		return *this;
	}
};

class Ptr : public PtrPad<void *> {
	template<class T> friend class PtrPad;
	typedef PtrPad<void *> Super;

	Ptr(PtrController &ctrl) : Super(&ctrl) { Super::iref(); }
public:
	Ptr() : Super(new PtrController(new char)) {}
	Ptr(const Ptr &init) : Super(init.ctrl) { Super::iref(); }
	~Ptr() { Super::divest(); }

	Ptr &operator=(const Ptr &other) {
		Super::assing(other.ctrl);
		return *this;
	}
	void *operator*() { return ctrl->pw->operator void *(); }

	template<class U>
	TPtr<U> ptr() {
		return TPtr<U>(*ctrl);
	}

	template<class U>
	TRef<U> ref() {
		return TRef<U>(*ctrl);
	}
};

class Ref : public PtrPad<void *> {
	template<class T> friend class PtrPad;
	typedef PtrPad<void *> Super;

	Ref(PtrController &ctrl) : Super(&ctrl) { Super::iref(); }
public:
	Ref() : Super(nullptr) {}
	Ref(const Ref &init) : Super(init.ctrl) { Super::iref(); }
	~Ref() = default;

	Ref &operator=(const Ref &other) {
		Super::assing(other.ctrl);
		return *this;
	}
	void *operator*() { return ctrl->pw->operator void *(); }

	template<class U>
	TPtr<U> ptr() {
		return TPtr<U>(*ctrl);
	}

	template<class U>
	TRef<U> ref() {
		return TRef<U>(*ctrl);
	}
};

template<class T>
auto PtrPad<T>::ptr() {
	return Ptr(*ctrl);
}
template<class T>
auto PtrPad<T>::ref() {
	return Ref(*ctrl);
}

#endif // !__pointer