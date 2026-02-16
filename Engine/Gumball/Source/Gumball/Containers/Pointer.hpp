#ifndef __pointer
#define __pointer
#include <concepts>
#include <type_traits>

namespace Pointer {
	/*this is analogous to shared_ptr and weak_ptr, plus allows abstraction to void*, 
	safety is not enforced, you better know what you are doing */

	struct IMemory {
		virtual ~IMemory() {}
		virtual operator void *() = 0;
	};
	
	template<class T>
	struct TMemory : public IMemory {
		T *ptr = nullptr;
		TMemory(T *init) : ptr(init) {}
		~TMemory() { delete ptr; ptr = nullptr; }
		operator void *() { return ptr; }
	};
	
	class Memory {
	public:
		unsigned refs = 1;
		IMemory *pw = nullptr;

		template<class U> Memory(U *init) : pw(new TMemory<U>(init)) {}
		~Memory() { delete pw; }
	};

	template<class T> class Ptr;
	template<class T> class Ref;

	template<class T, class U>
	concept InterOp = requires {
		std::is_convertible_v<T, U>;
		std::is_convertible_v<U, T>;
		std::is_same_v<U, void *>;
	};

	template<class T>
	class SmartPointer {
		using Memory = Pointer::Memory;
		template<class t> using TMemory = Pointer::TMemory<t>;
		template<class t> friend class SmartPointer;

	protected:
		Memory *mem;

		Inline void iref() { ++mem->refs; }
		Inline void dref() { --mem->refs; }
		Inline void delMem() { delete mem; mem = nullptr; }
		Inline void delPtr() { delete mem->pw; mem->pw = nullptr; }

		Inline void reference(Memory *other) { mem = other; }
		Inline void assing(Memory *other) { divest(); mem = other; iref(); }
		Inline void divest() { dref(); if (!mem->refs) { delMem(); } }
		SmartPointer(Memory *memInit = nullptr) : mem(memInit) {}

	public:
		SmartPointer() = delete;
		SmartPointer(SmartPointer &&) = delete;
		SmartPointer(const SmartPointer &) = delete;
		SmartPointer &operator=(SmartPointer &&other) = delete;
		SmartPointer &operator=(const SmartPointer &other) = delete;
		~SmartPointer() = default;

		template<class U> requires InterOp<T, U>
		bool operator==(const SmartPointer<U> &other) const {
			return mem == other.mem;
		}

		template<class U> requires InterOp<T, U>
		const U *As() const {
			return dynamic_cast<const U *>(static_cast<const TMemory<U>*>(mem->pw)->ptr);
		}
		template<class U>
		const U *To() const {
			return static_cast<const TMemory<U>*>(mem->pw)->ptr;
		}
		template<class U>
		U *To() {
			return static_cast<const TMemory<U>*>(mem->pw)->ptr;
		}
		template<class U>
		bool Is() const { static_cast<bool>(dynamic_cast<const U *>(static_cast<const TMemory<U>*>(mem->pw)->ptr)); }

		T &operator*() { return *To<T>(); }
		const T &operator*() const { return *To<T>(); }
		T *const operator->() { return To<T>(); }
		const T *const operator->() const { return To<T>(); }

		operator bool() const { return mem && mem->pw && To<T>(); }
		operator void *() const { return mem ? (*mem->pw) : nullptr; }
		
		template<class U> requires InterOp<T, U>
		operator Ptr<U>() 
		{
			return Ptr<U>(*mem);
		}
		
		template<class U>
		operator Ref<U>() requires InterOp<T, U> {
			return Ref<U>(*mem);
		}
	};

	template<class T>
	class Ptr : public SmartPointer<T> {
		template<class t> friend class SmartPointer;
		template<class t> friend class Ptr;
		typedef SmartPointer<T> Super;

		Ptr(Memory &mem) : Super(&mem) { Super::iref(); }
	
	public:
		Ptr(T *pInit = nullptr) : Super(new Pointer::Memory(pInit)) {}
		Ptr(const Ptr &init) : Super(init.mem) { Super::iref(); }
		~Ptr() { Super::divest(); }

		void operator~() {
			Super::delPtr();
		}
		Ptr &operator=(const Ptr &other) {
			Super::assing(other.mem);
			return *this;
		}
		const Ptr<T> &operator=(Ptr<T> &&other) {
			Super::assing(other.mem);
			return *this;
		}		
	};

	template<class T>
	class Ref : public SmartPointer<T> {
		template<class t> friend class SmartPointer;
		template<class t> friend class Ref;
		typedef SmartPointer<T> Super;

		Ref(Pointer::Memory &mem) : Super(&mem) {}

	public:
		Ref() : Super(nullptr) {}
		Ref(const Ref &init) : Super(init.mem) {}
		~Ref() = default;

		Ref &operator=(const Ref &other) {
			Super::reference(other.mem);
			return *this;
		}
		const Ref<T> &operator=(Ref<T> &&other) {
			Super::reference(other.mem);
			return *this;
		}
	};


	template<>
	class Ptr<void *> : public SmartPointer<void *> {
		template<class T> friend class SmartPointer;
		typedef SmartPointer<void *> Super;

		Ptr(Pointer::Memory &ctrl) : Super(&ctrl) { Super::iref(); }
	public:
		Ptr() : Super(new Pointer::Memory(static_cast<void *>(nullptr))) {}
		Ptr(const Ptr &init) : Super(init.mem) { Super::iref(); }
		~Ptr() { Super::divest(); }

		template<class U> 
		Ptr &operator=(const Ptr<U> &other)
		{
			Super::assing(other.mem);
			return *this;
		}
	};

	template<>
	class Ref<void *> : public SmartPointer<void *> {
		template<class T> friend class SmartPointer;
		typedef SmartPointer<void *> Super;

		Ref(Pointer::Memory &mem) : Super(&mem) { }
	public:
		Ref() : Super(nullptr) {}
		Ref(const Ref &init) : Super(init.mem) { }
		~Ref() = default;

		Ref &operator=(const Ref &other) {
			Super::reference(other.mem);
			return *this;
		}

		template<class U>
		operator Ptr<U>() {
			return Ptr<U>(*mem);
		}
		template<class U>
		operator Ref<U>() {
			return Ref<U>(*mem);
		}
	};
};

template<class T> using Ptr = Pointer::Ptr<T>;
template<class T> using Ref = Pointer::Ref<T>;
using PtrWeak = Ptr<void *>;
using RefWeak = Ref<void *>;

#endif // !__pointer