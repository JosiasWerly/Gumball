#pragma once
#ifndef __flowcommon
#define __flowcommon

#include <unordered_map>

namespace Flow {
	enum class eStatus : char {
		nominal, success, fail
	};

	struct TInt {
		unsigned val = 0;

		TInt() = default;
		TInt operator=(TInt other) { val = other.val; return *this; }
		
		template<class T> TInt(T value = 0) : val(int(value)) {}
		template<class T> operator T() { return static_cast<T>(val); }
		template<class T> operator const T() const { return static_cast<T>(val); }
		bool operator==(TInt other) const { return val == other.val; }
	};
	
	struct TIntOperators {
		std::size_t operator()(const TInt &key) const { return std::hash<int>()(key.val); }
		bool operator()(const TInt &a, const TInt &b) const { return a.val == b.val; }
	};
	
	template<class T>
	using TIntMap = std::unordered_map<TInt, T, TIntOperators>;
};
#endif // !__flowcommon