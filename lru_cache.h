#pragma once

#include <utility>
#include <list>
#include <unordered_map>
#include <assert.h>
#include <sstream>

template<typename KeyType, typename ValueType>
class LRUCache {
public:
    LRUCache(size_t max_cache_size) : _max_cache_size(max_cache_size) {};
    ~LRUCache() = default;

    bool get(const KeyType& key, ValueType& value) {
        const auto cache_map_iter = _cache_map.find(key);
        if (cache_map_iter == _cache_map.end()) {
            // 没找到
            return false;
        }

        // 找到了
        // 得到结果，赋值给value
        const auto cache_list_iter = cache_map_iter->second;
        assert(cache_list_iter->first == key);
        value = cache_list_iter->second;

        // 将该缓存数据移至list头部，同时更新map中对应key的迭代器
        _cache_list.erase(cache_list_iter);
        _cache_list.emplace_front(std::pair<KeyType, ValueType>(key, value));
        _cache_map[key] = _cache_list.begin();
        return true;
    }

    void set(const KeyType& key, const ValueType& value) {
        const auto cache_map_iter = _cache_map.find(key);
        if (cache_map_iter == _cache_map.end()) {
            // 没找到
            if (_cache_list.size() >= _max_cache_size) {
                // 如果已经达到缓存容量最大值
                // 将list尾部数据移除，并将该数据从map中同步移除
                assert(_max_cache_size != 0);
                const auto cache_list_iter = --_cache_list.end();
                _cache_map.erase(cache_list_iter->first);
                _cache_list.erase(cache_list_iter);
            }

            // 将新数据插入到list头部，同时更新map中对应key的迭代器
            _cache_list.emplace_front(std::pair<KeyType, ValueType>(key, value));
            _cache_map[key] = _cache_list.begin();
        } else {
            // 找到了
            // 将该缓存数据移至list头部，同时更新map中对应key的迭代器
            const auto cache_list_iter = cache_map_iter->second;
            assert(cache_list_iter->first == key);
            _cache_list.erase(cache_list_iter);
            _cache_list.emplace_front(std::pair<KeyType, ValueType>(key, value));
            _cache_map[key] = _cache_list.begin();
        }
    }

    std::string cache_list_str() const {
        std::stringstream ss;
        ss << "cache_list:";
        for (const auto& cache_list_pair : _cache_list) {
            ss << " (" << cache_list_pair.first << ", " << cache_list_pair.second << ")";
        }
        ss << "\n";
        return ss.str();
    }

private:
    size_t _max_cache_size;
    std::list<std::pair<KeyType, ValueType>> _cache_list;
    std::unordered_map<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> _cache_map;
};
