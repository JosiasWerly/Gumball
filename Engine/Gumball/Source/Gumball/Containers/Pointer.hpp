#ifndef __pointer
#define __pointer
#include <type_traits>

namespace Pointer {
//slow but gold, i.e. it make things easier

template<class T> class Ptr;
template<class T> class Ref;

template<class T, class U>
concept InterOp = requires {
	std::is_convertible_v<T, U>;
	std::is_convertible_v<U, T>;
	std::is_same_v<U, void *>;
};

struct IValue {
	//only way to support Ptr<void>
	virtual ~IValue() {}
	virtual void *Value() = 0;
};

template<class T>
struct TValue : public IValue {
	T *value = nullptr;

	TValue(T *init = new T) : value(init) {}
	~TValue() { delete value; }
	void *Value() { return value; };
};

struct Memory {
	unsigned ref = 1;
	IValue *ptr = nullptr;

	template<class T>
	Memory(T *init) : ptr(new TValue<T>(init)) {}
	~Memory() { delete ptr; }
};

template<class T>
class SmartPointer {
	template<class u> friend class SmartPointer;
	using Value = TValue<T>;

protected:
	Memory *mem = nullptr;

	Inline void IncRef() { ++mem->ref; }
	Inline void DecRef() { --mem->ref; }
	Inline void DelMem() { delete mem; mem = nullptr; }
	Inline void Assing(Memory *other) { Divest(); mem = other; IncRef(); }
	Inline void Divest() { if (!mem) return; DecRef(); if (!mem->ref) { DelMem(); } }
	Inline void Set(Memory *other) { mem = other; }
	Inline void Move(Memory *&other) { if (mem) DelMem(); mem = other; other = nullptr; }
	Inline T *Val() { return reinterpret_cast<T *>(mem->ptr->Value()); }
	Inline const T *Val() const { return reinterpret_cast<const T *>(mem->ptr->Value()); }

	SmartPointer(Memory *init = nullptr) : mem(init) {}

public:
	SmartPointer() = delete;
	SmartPointer(SmartPointer &&) = delete;
	SmartPointer(const SmartPointer &) = delete;
	SmartPointer &operator=(SmartPointer &&other) = delete;
	SmartPointer &operator=(const SmartPointer &other) = delete;
	~SmartPointer() = default;

	T &operator*() { return *Val(); }
	const T &operator*() const { return *Val(); }
	T *const operator->() { return Val(); }
	const T *const operator->() const { return Val(); }

	template<class U> requires InterOp<T, U>
	bool operator==(const SmartPointer<U> &other) const { return mem == other.mem; }
	operator bool() const { return mem && Val(); }
	operator void *() { return mem ? Val() : nullptr; }
	operator const void *() const { return mem ? Val() : nullptr; }

	template<class U> requires InterOp<T, U>
	const U *To() const { return dynamic_cast<const U *>(Val()); }

	template<class U>
	bool Is() const { static_cast<bool>(dynamic_cast<const U *>(Val())); }

	template<class U> requires InterOp<T, U>
	operator Ptr<U>() { return Ptr<U>(*mem); }

	template<class U> requires InterOp<T, U>
	operator Ref<U>() { return Ref<U>(*mem); }
};

template<class T>
class Ptr : public SmartPointer<T> {
	template<class t> friend class SmartPointer;
	template<class t> friend class Ptr;
	using Super = SmartPointer<T>;

	Ptr(Memory &mem) : Super(&mem) { if (Super::mem) Super::IncRef(); }

public:
	Ptr() : Super(nullptr) {}
	Ptr(T *pInit) : Super(new Memory(pInit)) {}
	Ptr(const Ptr &init) : Super(init.mem) { Super::IncRef(); }
	~Ptr() { Super::Divest(); }

	void operator~() { Super::DelPtr(); }
	Ptr &operator=(const Ptr &other) {
		Super::Assing(other.mem);
		return *this;
	}
	const Ptr<T> &operator=(Ptr<T> &&other) noexcept {
		Super::Assing(other.mem);
		return *this;
	}
};

template<class T>
class Ref : public SmartPointer<T> { 
	template<class t> friend class SmartPointer;
	template<class t> friend class Ref;
	using Super = SmartPointer<T>;

	Ref(Memory &mem) : Super(&mem) {}

public:
	Ref() : Super(nullptr) {}
	Ref(const Ref &init) : Super(init.mem) {}
	~Ref() = default;

	Ref &operator=(const Ref &other) {
		Super::Set(other.mem);
		return *this;
	}
	const Ref<T> &operator=(Ref<T> &&other) {
		Super::Set(other.mem);
		return *this;
	}
};

template<class T>
class Unq : public SmartPointer<T> {
	template<class t> friend class SmartPointer;
	template<class t> friend class Ref;
	using Super = SmartPointer<T>;

	Unq(Memory &mem) : Super(&mem) {}

public:
	Unq() : Super(nullptr) {}
	Unq(T *pInit) : Super(new Memory(pInit)) {}
	Unq(const Unq &init) : Super(init.mem) { const_cast<Unq &>(init).Set(nullptr); }
	~Unq() { if (Super::mem) Super::DelMem(); }

	Unq &operator=(const Unq &other) {
		Super::Move(other.mem);
		return *this;
	}

	template<class U> operator Ref<U>() = delete;
	template<class U> requires InterOp<T, U>
	operator Ptr<U>() {
		Memory *m = Super::mem;
		Super::Set(nullptr);
		return Ptr<U>(*m);
	}
};

template<>
class Ptr<void> : public SmartPointer<void *> { 
	template<class T> friend class SmartPointer;
	using Super = SmartPointer<void *>;

	Ptr(Memory &mem) : Super(&mem) { if (Super::mem) Super::IncRef(); }

public:
	Ptr() : Super(nullptr) {}
	Ptr(const Ptr &init) : Super(init.mem) { if (mem) Super::IncRef(); }
	~Ptr() { Super::Divest(); }

	template<class U>
	Ptr &operator=(const Ptr<U> &other) {
		Super::Assing(other.mem);
		return *this;
	}

	template<class U>
	const U *To() const { return reinterpret_cast<const U *>(Val()); }
};

};

template<class T> using Ptr = Pointer::Ptr<T>; //shared pointer
template<class T> using Ref = Pointer::Ref<T>; //weak pointer
template<class T> using Unq = Pointer::Unq<T>; //unique pointer
using Void = Ptr<void>; // you better know what you are doing
#endif // !__pointer