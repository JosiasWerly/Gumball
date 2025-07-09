#pragma once
#ifndef __flowcommon
#define __flowcommon

#include <bitset>
#include <unordered_map>
#include <deque>

namespace Flow {
	struct THash {
		int val = 0;

		THash() = default;
		THash(const THash &other) : val(other.val) {}
		THash(THash &&other) noexcept : val(other.val) {}
		template<class T> THash(T value) : val(int(value)) {}
		template<class T> operator T() { return T(val); }
		template<class T> operator const T() const { return T(val); }

		bool operator==(const THash &other) const { return val == other.val; }
		operator bool() const { return val; }
		operator int() const { return val; }
	};
	struct THashOperations {
		std::size_t operator()(const THash &key) const { return std::hash<int>()(key.val); }
		bool operator()(const THash &a, const THash &b) const { return a.val == b.val; }
	};

	template<class T>
	class Node {
		T val;
		std::unordered_map<THash, Node<T>, THashOperations> nodes;

	public:
		T &pin() { return val; }
		const T &pin() const { return val; }
		T *operator->() { return &val; }
		const T *operator->() const { return &val; }
		Node &operator[](THash key) { return nodes[key]; }
		const Node &operator[](THash key) const { return nodes.at(key); }

		class Nav {
			const std::deque<THash> ls;
		
		public:
			constexpr Nav(std::initializer_list<THash> ls) : ls(ls) {}
			Node *find(Node *root) {
				auto it = ls.begin();
				while (it != ls.end()) {
					root = &root[*(it)];
					++it;
				}
				return root;
			}
		};
	};
};
#endif // !__flowcommon