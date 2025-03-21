#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <sstream>
#include <string>
  
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable
{
    class HashNode {
    public:
        Key key;
        Value value;
  
        explicit HashNode(const Key &key) : key(key), value() {}
        HashNode(const Key &key, const Value &value) : key(key), value(value) {}
  
        bool operator==(const HashNode &other) const {
            return key == other.key;
        }
  
        bool operator!=(const HashNode &other) const {
            return key != other.key;
        }
  
        bool operator<(const HashNode &other) const {
            return key < other.key;
        }
  
        bool operator>(const HashNode &other) const {
            return key > other.key;
        }
  
        bool operator==(const Key &key_) {
            return key == key_;
        }
    };
  
private:
    using Bucket = std::list<HashNode>;
    std::vector<Bucket> buckets;
    Hash hashFunction;
    size_t tableSize;
    size_t numElements;
  
    float maxLoadFactor = 0.75;
  
    size_t hash(const Key &key) const { return hashFunction(key) % tableSize; }
  
    void rehash(size_t newSize) {
        std::vector<Bucket> newBuckets(newSize);
        for (Bucket &bucket : buckets) {
            for (HashNode &hashNode : bucket) {
                size_t newIndex = hashFunction(hashNode.key) % newSize;
                newBuckets[newIndex].push_back(hashNode);
            }
        }
        buckets = std::move(newBuckets);
        tableSize = newSize;
    }
  
public:
    HashTable(size_t size = 10, const Hash &hashFunc = Hash()) : buckets(size), hashFunction(hashFunc), tableSize(size), numElements(0) {}
  
    void insert(const Key &key, const Value &value) {
        if ((numElements + 1) > maxLoadFactor * tableSize) {
            if (tableSize == 0) tableSize = 1;
            rehash(tableSize * 2);
        }
        size_t index = hash(key);
        std::list<HashNode> &bucket = buckets[index];
        if (std::find(bucket.begin(), bucket.end(), key) == bucket.end()) {
            bucket.push_back(HashNode(key, value));
            ++numElements;
        }
    }
  
    void insertKey(const Key &key) { insert(key, Value{}); }
  
    void erase(const Key &key) {
        size_t index = hash(key);
        auto &bucket = buckets[index];
        auto it = std::find(bucket.begin(), bucket.end(), key);
        if (it != bucket.end()) {
            bucket.erase(it);
            numElements--;
        }
    }
  
    Value *find(const Key &key) {
        size_t index = hash(key);
        auto &bucket = buckets[index];
        auto it = std::find(bucket.begin(), bucket.end(), key);
        if (it != bucket.end()) {
            return &it->value;
        };
        return nullptr;
    }
  
    size_t size() const { return numElements; }
  
    void clear() {
        this->buckets.clear();
        this->numElements = 0;
        this->tableSize = 0;
    }
};
