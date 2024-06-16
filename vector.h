#ifndef VECTOR_H_
#define VECTOR_H_

#define VECTOR_MEMORY_IMPLEMENTED

#include <cstdlib>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <utility>
#include <memory>

template <class T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  const int kExpandCf = 2;

  // Main constructors
  Vector() : store_(nullptr), size_(0), capacity_(0){};

  explicit Vector(size_t size) : size_(size), capacity_(size) {
    if (size == 0) {
      store_ = nullptr;
      return;
    }
    store_ = Allocate(size);
    for (size_t i = 0; i < size; ++i) {
      try {
        new (store_ + i) T();
      } catch (...) {
        Deallocate(i);
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  Vector(const size_t size, const T& value) : size_(size), capacity_(size) {
    if (size == 0) {
      store_ = nullptr;
      return;
    }
    store_ = Allocate(size);
    for (size_t i = 0; i < size; ++i) {
      try {
        new (store_ + i) T(value);
      } catch (...) {
        Deallocate(i);
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
      std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    store_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    try {
      for (auto it = first; it != last; it++) {
        PushBack(*it);
      }
    } catch (...) {
      Deallocate(size_);
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  Vector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()) {
    store_ = Allocate(size_);
    size_t padding = 0;
    for (auto it = init.begin(); it != init.end(); ++it) {
      try {
        new (store_ + padding++) T(*it);
      } catch (...) {
        Deallocate(padding - 1);
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  // Copy constructor
  Vector(const Vector<T>& copy) : size_(copy.size_), capacity_(copy.capacity_) {
    store_ = Allocate(copy.capacity_);
    for (size_t i = 0; i < size_; ++i) {
      try {
        new (store_ + i) T(copy.At(i));
      } catch (...) {
        Deallocate(i);
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  // Move constructor
  Vector(Vector<T>&& move) noexcept : store_(move.store_), size_(move.size_), capacity_(move.capacity_) {
    move.store_ = nullptr;
    move.size_ = 0;
    move.capacity_ = 0;
  }

  // Destructor
  ~Vector() {
    Deallocate(size_);
  }

  // Copy assignment operator
  Vector<T>& operator=(const Vector<T>& copy) {
    if (this == &copy) {
      return *this;
    }
    Deallocate(size_);
    size_ = copy.size_;
    capacity_ = copy.capacity_;
    store_ = Allocate(size_);
    for (size_t i = 0; i < size_; ++i) {
      try {
        new (store_ + i) T(copy[i]);
      } catch (...) {
        Deallocate(i);
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
    return *this;
  }

  // Move assignment operator
  Vector<T>& operator=(Vector<T>&& move) noexcept {
    if (this == &move) {
      return *this;
    }
    Deallocate(size_);
    store_ = move.store_;
    size_ = move.size_;
    capacity_ = move.capacity_;
    move.store_ = nullptr;
    move.size_ = 0;
    move.capacity_ = 0;
    return *this;
  }

  // Methods
  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  bool Empty() const {
    return size_ == 0;
  }

  T& operator[](const size_t idx) {
    return *(store_ + idx);
  }

  const T& operator[](const size_t idx) const {
    return *(store_ + idx);
  }

  T& At(const size_t idx) {
    if (idx >= Size()) {
      throw std::out_of_range("Index overcomes border");
    }
    return *(store_ + idx);
  }

  const T& At(const size_t idx) const {
    if (idx >= Size()) {
      throw std::out_of_range("Index overcomes border");
    }
    return *(store_ + idx);
  }

  T& Front() {
    return *store_;
  }

  const T& Front() const {
    return *store_;
  }

  T& Back() {
    return *(store_ + size_ - 1);
  }

  const T& Back() const {
    return *(store_ + size_ - 1);
  }

  T* Data() {
    return store_;
  }

  const T* Data() const {
    return store_;
  }

  void Clear() {
    if (store_ != nullptr) {
      std::destroy_n(store_, size_);
    }
    size_ = 0;
  }

  void Swap(Vector<T>& other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(store_, other.store_);
  }

  void Resize(size_t new_size) {
    if (new_size <= size_) {
      if (new_size < size_) {
        std::destroy_n(store_ + new_size, size_ - new_size);
        size_ = new_size;
      }
      return;
    }
    if (new_size <= capacity_) {
      for (size_t i = size_; i < new_size; ++i) {
        try {
          new (store_ + i) T();
        } catch (...) {
          std::destroy_n(store_ + size_, i - size_);
          throw;
        }
      }
      size_ = new_size;
      return;
    }
    T* new_store = Allocate(new_size);
    for (size_t i = size_; i < new_size; ++i) {
      try {
        new (new_store + i) T();
      } catch (...) {
        std::destroy_n(new_store + size_, i - size_);
        auto void_ptr = static_cast<void*>(new_store);
        auto align = static_cast<std::align_val_t>(alignof(T));
        ::operator delete(void_ptr, align);
        throw;
      }
    }
    std::uninitialized_move_n(store_, size_, new_store);
    Deallocate(size_);
    store_ = new_store;
    capacity_ = new_size;
    size_ = new_size;
  }

  void Resize(size_t new_size, T value) {
    if (new_size <= size_) {
      if (new_size < size_) {
        std::destroy_n(store_ + new_size, size_ - new_size);
        size_ = new_size;
      }
      return;
    }
    if (new_size <= capacity_) {
      for (size_t i = size_; i < new_size; ++i) {
        try {
          new (store_ + i) T(value);
        } catch (...) {
          std::destroy_n(store_ + size_, i - size_);
          throw;
        }
      }
      size_ = new_size;
      return;
    }
    T* new_store = Allocate(new_size);
    for (size_t i = size_; i < new_size; ++i) {
      try {
        new (new_store + i) T(value);
      } catch (...) {
        std::destroy_n(new_store + size_, i - size_);
        auto void_ptr = static_cast<void*>(new_store);
        auto align = static_cast<std::align_val_t>(alignof(T));
        ::operator delete(void_ptr, align);
        throw;
      }
    }
    std::uninitialized_move_n(store_, size_, new_store);
    Deallocate(size_);
    store_ = new_store;
    capacity_ = new_size;
    size_ = new_size;
  }

  void Reserve(size_t new_cap) {
    if (new_cap <= capacity_) {
      return;
    }
    T* new_store = Allocate(new_cap);
    if (store_ != nullptr) {
      std::uninitialized_move_n(store_, size_, new_store);
      Deallocate(size_);
    }
    capacity_ = new_cap;
    store_ = new_store;
  }

  void ShrinkToFit() {
    if (capacity_ == 0) {
      return;
    }
    if (size_ == 0) {
      Deallocate(size_);
      capacity_ = 0;
      store_ = nullptr;
      return;
    }
    T* shrink_store = Allocate(size_);
    std::uninitialized_move_n(store_, size_, shrink_store);
    Deallocate(size_);
    capacity_ = size_;
    store_ = shrink_store;
  }

  void PushBack(const T& value) {
    T* new_store = nullptr;
    auto new_capacity = capacity_ == 0 ? 1 : kExpandCf * capacity_;
    if (size_ == capacity_) {
      new_store = Allocate(new_capacity);
    } else {
      new_store = store_;
    }
    try {
      new (new_store + size_) T(value);
      ++size_;
      if (new_store != store_) {
        std::uninitialized_move_n(store_, size_ - 1, new_store);
        Deallocate(size_ - 1);
        store_ = new_store;
        capacity_ = new_capacity;
      }
    } catch (...) {
      if (new_store != store_) {
        auto void_ptr = static_cast<void*>(new_store);
        auto align = static_cast<std::align_val_t>(alignof(T));
        ::operator delete(void_ptr, align);
      }
      throw;
    }
  }

  void PushBack(T&& value) {
    if (size_ == capacity_) {
      Reserve(capacity_ == 0 ? 1 : capacity_ * kExpandCf);
    }
    try {
      new (store_ + size_) T(std::forward<T&&>(value));
      ++size_;
    } catch (...) {
    }
  }

  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    T* new_store = nullptr;
    auto new_cap = capacity_ == 0 ? 1 : kExpandCf * capacity_;
    if (size_ == capacity_) {
      new_store = Allocate(new_cap);
    } else {
      new_store = store_;
    }
    try {
      new (new_store + size_) T(std::forward<Args&&>(args)...);
      ++size_;
      if (new_store != store_) {
        std::uninitialized_move_n(store_, size_ - 1, new_store);
        Deallocate(size_ - 1);
        store_ = new_store;
        capacity_ = new_cap;
      }
    } catch (...) {
      if (new_store != store_) {
        auto void_ptr = static_cast<void*>(new_store);
        auto align = static_cast<std::align_val_t>(alignof(T));
        ::operator delete(void_ptr, align);
      }
      throw;
    }
  }

  void PopBack() {
    if (size_ == 0) {
      return;
    }
    std::destroy_at(store_ + size_ - 1);
    size_--;
  }

  // Compare operators
  friend bool operator==(const Vector& left, const Vector& right) {
    if (left.Size() != right.Size()) {
      return false;
    }
    for (size_t i = 0; i < left.Size(); ++i) {
      if (left[i] != right[i]) {
        return false;
      }
    }
    return true;
  }

  friend bool operator!=(const Vector& left, const Vector& right) {
    return !(left == right);  // NOLINT
  }

  friend bool operator<(const Vector& left, const Vector& right) {
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
  }

  friend bool operator<=(const Vector& left, const Vector& right) {
    return !(right < left);  // NOLINT
  }

  friend bool operator>(const Vector& left, const Vector& right) {
    return right < left;  // NOLINT
  }

  friend bool operator>=(const Vector& left, const Vector& right) {
    return !(left < right);  // NOLINT
  }

  // Iterators

  Iterator begin() {  // NOLINT
    return store_;
  }

  ConstIterator begin() const {  // NOLINT
    return store_;
  }

  ConstIterator cbegin() const {  // NOLINT
    return store_;
  }

  Iterator end() {  // NOLINT
    return store_ + size_;
  }

  ConstIterator end() const {  // NOLINT
    return store_ + size_;
  }

  ConstIterator cend() const {  // NOLINT
    return store_ + size_;
  }

  ReverseIterator rbegin() {  // NOLINT
    return std::make_reverse_iterator(end());
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return std::make_reverse_iterator(cend());
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return std::make_reverse_iterator(cend());
  }

  ReverseIterator rend() {  // NOLINT
    return std::make_reverse_iterator(begin());
  }

  ConstReverseIterator rend() const {  // NOLINT
    return std::make_reverse_iterator(cbegin());
  }

  ConstReverseIterator crend() const {  // NOLINT
    return std::make_reverse_iterator(cbegin());
  }

 private:
  T* store_;
  size_t size_;
  size_t capacity_;

  T* Allocate(size_t num) {
    if (num == 0) {
      return nullptr;
    }
    auto total_memory = num * sizeof(T);
    auto align = static_cast<std::align_val_t>(alignof(T));
    auto void_ptr = ::operator new(total_memory, align);
    return static_cast<T*>(void_ptr);
  }

  void Deallocate(size_t amount) {
    if (store_ == nullptr) {
      return;
    }
    std::destroy_n(store_, amount);
    auto void_ptr = static_cast<void*>(store_);
    auto align = static_cast<std::align_val_t>(alignof(T));
    ::operator delete(void_ptr, align);
    store_ = nullptr;
  }
};

#endif  // VECTOR_H_

