#ifndef ARRAY_H_
#define ARRAY_H_

#define ARRAY_TRAITS_IMPLEMENTED

#include <cstddef>
#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

template <typename T, size_t S>
class Array {
 public:
  T store_[S];

  T& operator[](size_t idx) {
    return store_[idx];
  }

  const T& operator[](size_t idx) const {
    return store_[idx];
  }

  T& At(size_t idx) {
    if (idx >= S) {
      throw ArrayOutOfRange();
    }
    return store_[idx];
  }

  const T& At(size_t idx) const {
    if (idx >= S) {
      throw ArrayOutOfRange();
    }
    return store_[idx];
  }

  T& Front() {
    if (S == 0) {
      throw ArrayOutOfRange();
    }
    return store_[0];
  }

  const T& Front() const {
    if (S == 0) {
      throw ArrayOutOfRange();
    }
    return store_[0];
  }

  T& Back() {
    if (S == 0) {
      throw ArrayOutOfRange();
    }
    return store_[S - 1];
  }

  const T& Back() const {
    if (S == 0) {
      throw ArrayOutOfRange();
    }
    return store_[S - 1];
  }

  T* Data() {
    if (S == 0) {
      throw ArrayOutOfRange();
    }
    return &store_[0];
  }

  const T* Data() const {
    if (S == 0) {
      throw ArrayOutOfRange();
    }
    return &store_[0];
  }

  size_t Size() const {
    return S;
  }

  bool Empty() const {
    return S == 0;
  }

  void Fill(const T& value) {
    for (size_t i = 0; i < S; ++i) {
      store_[i] = value;
    }
  }

  void Swap(Array<T, S>& other) {
    for (size_t i = 0; i < S; ++i) {
      std::swap(store_[i], other.store_[i]);
    }
  }
};

// Additional

// Size
template <typename T>
size_t GetSize(const T&) {
  return 0;
}

template <typename T, size_t S>
size_t GetSize(const T (&)[S]) {
  return S;
}

// Rank
template <typename T>
size_t GetRank(const T&) {
  return 0;
}

template <typename T, size_t S>
size_t GetRank(const T (&)[S]) {
  return GetRank(T{}) + 1;
}

// NumElements
template <typename T>
size_t GetNumElements(const T&) {
  return 1;
}

template <typename T, size_t S>
size_t GetNumElements(const T (&)[S]) {
  return GetNumElements(T{}) * S;
}

#endif  // ARRAY_H_
