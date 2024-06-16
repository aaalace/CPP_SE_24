#ifndef UNORDERED_SET_H_
#define UNORDERED_SET_H_

#include <vector>
#include <list>
#include <functional>
#include <utility>

const float kMaxLoadFactor = 1.0;

template <class KeyT>
class UnorderedSet {
 public:
  // Constructors
  UnorderedSet() = default;

  explicit UnorderedSet(size_t count) : table_(count) {
  }

  template <typename It>
  UnorderedSet(It begin, It end) : n_elements_(0) {
    size_t distance = std::distance(begin, end);
    table_.resize(distance);
    for (auto it = begin; it != end; ++it) {
      auto val = *it;
      table_[Hash(val)].push_back(val);
    }
    n_elements_ += distance;
  }

  UnorderedSet(const UnorderedSet<KeyT>& other) : table_(other.table_), n_elements_(other.n_elements_) {
  }

  UnorderedSet(UnorderedSet<KeyT>&& other) noexcept : table_(std::move(other.table_)), n_elements_(other.n_elements_) {
    other.n_elements_ = 0;
  }

  // Copy/move assigns
  UnorderedSet<KeyT>& operator=(const UnorderedSet<KeyT>& other) {
    if (this != &other) {
      table_ = other.table_;
      n_elements_ = other.n_elements_;
    }
    return *this;
  }

  UnorderedSet<KeyT>& operator=(UnorderedSet<KeyT>&& other) noexcept {
    if (this != &other) {
      table_ = std::move(other.table_);
      n_elements_ = other.n_elements_;
      other.n_elements_ = 0;
    }
    return *this;
  }

  // Destructor
  ~UnorderedSet() {
    Clear();
  }

  // Methods
  size_t Size() const {
    return n_elements_;
  }

  bool Empty() const {
    return n_elements_ == 0;
  }

  size_t BucketCount() const {
    return table_.size();
  }

  size_t BucketSize(const size_t id) const {
    if (id >= BucketCount()) {
      return 0;
    }
    return table_[id].size();
  }

  size_t Bucket(const KeyT& key) const {
    return Hash(key);
  }

  double LoadFactor() const {
    return n_elements_ / std::max(static_cast<double>(table_.size()), 1.0);
  }

  void Clear() {
    table_.clear();
    n_elements_ = 0;
  }

  bool Find(const KeyT& key) const {
    if (BucketCount() == 0) {
      return false;
    }
    auto hash = Hash(key);
    return std::find(table_[hash].begin(), table_[hash].end(), key) != table_[hash].end();
  }

  bool Insert(const KeyT& key) {
    if (Find(key)) {
      return false;
    }
    if (BucketCount() == 0) {
      Reserve(1);
    }
    if (LoadFactor() >= kMaxLoadFactor) {
      Reserve(Size() * 2);
    }
    size_t index = Hash(key);
    table_[index].push_back(key);
    ++n_elements_;
    return true;
  }

  bool Insert(KeyT&& key) {
    return Insert(std::as_const(key));
  }

  bool Erase(const KeyT& key) {
    auto& bucket = table_[Hash(key)];
    auto it = std::find(bucket.begin(), bucket.end(), key);
    if (it != bucket.end()) {
      bucket.erase(it);
      --n_elements_;
      return true;
    }
    return false;
  }

  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count < n_elements_) {
      return;
    }
    std::vector<std::list<KeyT>> new_buckets(new_bucket_count);
    for (const auto& bucket : table_) {
      for (const auto& elem : bucket) {
        size_t new_index = std::hash<KeyT>{}(elem) % new_bucket_count;
        new_buckets[new_index].push_back(elem);
      }
    }
    table_.swap(new_buckets);
  }

  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count <= BucketCount()) {
      return;
    }
    Rehash(new_bucket_count);
  }

 private:
  std::vector<std::list<KeyT>> table_;
  size_t n_elements_ = 0;

  size_t Hash(const KeyT& val) const {
    return !table_.empty() ? std::hash<KeyT>{}(val) % table_.size() : 0;
  }
};

#endif  // UNORDERED_SET_H_
