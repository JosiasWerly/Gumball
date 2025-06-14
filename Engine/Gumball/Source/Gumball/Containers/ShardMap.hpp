#ifndef __shardmap
#define __shardmap

#include <list>
#include <unordered_map>
#include <deque>

template <class TKey, class TValue>
class ShardMap {
    struct Pool {
        std::list<TValue> data;
        std::unordered_map<TValue, typename std::list<TValue>::iterator> table;
    };
    std::unordered_map<TKey, Pool> pools;

public:
    const std::list<TValue> *get(TKey k) const {
        auto it = pools.find(k);
        return (it != pools.end()) ? &it->second.data : nullptr;
    }
    const std::list<TValue> &operator[](TKey k) const {
        auto it = pools.find(k);
        if (it == pools.end())
            throw;
        return it->second.data;
    }
    void push(TKey k, const TValue &v) {
        Pool &pool = pools[k];
        if (pool.table.find(v) != pool.table.end())
            return;  // Prevent duplicate entries
        pool.data.push_back(v);
        auto it = std::prev(pool.data.end());
        pool.table[v] = it;
    }
    void pop(TKey k, const TValue &v) {
        auto pIt = pools.find(k);
        if (pIt == pools.end()) return;

        Pool &pool = pIt->second;
        auto it = pool.table.find(v);
        if (it == pool.table.end()) return;

        pool.data.erase(it->second);
        pool.table.erase(it);
    }
    void move(TKey from, TKey to, const TValue &val) {
        pop(from, val);
        push(to, val);
    }
    void clear(TKey k) {
        pools.erase(k);
    }
    void clear() {
        pools.clear();
    }
    std::deque<TKey> keys() const {
        std::deque<TKey> out;
        for (const auto &[k, _] : pools)
            out.push_back(k);
        return out;
    }
    auto begin() { return pools.begin(); }
    auto end() { return pools.end(); }
    auto begin() const { return pools.cbegin(); }
    auto end() const { return pools.cend(); }
};
#endif // !__shardmap