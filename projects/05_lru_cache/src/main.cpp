#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

template <typename Key, typename Value>
class LruCache {
    using Entry = std::pair<Key, Value>;
    using List = std::list<Entry>;
    using Iterator = typename List::iterator;

public:
    explicit LruCache(std::size_t capacity) : capacity_(capacity) {}

    std::optional<Value> get(const Key& key) {
        const auto it = index_.find(key);
        if (it == index_.end()) {
            return std::nullopt;
        }

        // Moving the existing node is O(1); no copy of Value is needed here.
        items_.splice(items_.begin(), items_, it->second);
        return it->second->second;
    }

    void put(Key key, Value value) {
        if (capacity_ == 0) {
            return;
        }

        if (const auto it = index_.find(key); it != index_.end()) {
            it->second->second = std::move(value);
            items_.splice(items_.begin(), items_, it->second);
            return;
        }

        items_.emplace_front(std::move(key), std::move(value));
        index_[items_.front().first] = items_.begin();

        if (items_.size() > capacity_) {
            auto last = std::prev(items_.end());
            index_.erase(last->first);
            items_.pop_back();
        }
    }

    void dump() const {
        std::cout << "MRU -> LRU: ";
        for (const auto& [key, value] : items_) {
            std::cout << '(' << key << ':' << value << ") ";
        }
        std::cout << '\n';
    }

private:
    std::size_t capacity_;
    List items_;
    std::unordered_map<Key, Iterator> index_;
};

int main() {
    LruCache<std::string, int> cache(2);
    cache.put("a", 1);
    cache.put("b", 2);
    cache.dump();

    std::cout << "a = " << *cache.get("a") << '\n';
    cache.put("c", 3); // b is now the least recently used entry.
    cache.dump();
    std::cout << "b exists? " << std::boolalpha << cache.get("b").has_value() << '\n';
}
