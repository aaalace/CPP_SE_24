#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#define WEAK_PTR_IMPLEMENTED

#include <cstdlib>
#include <functional>
#include <memory>

// BadWeakPtr exception
class BadWeakPtr : public std::exception_ptr {
 public:
  BadWeakPtr() : std::exception_ptr() {
  }
};

// COUNTER
class Counter {
 public:
  bool EmptyStrong() {
    return strong_count_ == 0;
  }

  size_t GetStrong() {
    return strong_count_;
  }

  void AddStrong() {
    strong_count_++;
  }

  void RmStrong() {
    strong_count_--;
  }

  bool EmptyWeak() {
    return weak_count_ == 0;
  }

  size_t GetWeak() {
    return weak_count_;
  }

  void AddWeak() {
    weak_count_++;
  }

  void RmWeak() {
    weak_count_--;
  }

 private:
  size_t strong_count_ = 0;
  size_t weak_count_ = 0;
};

template <class T>
class WeakPtr;

// SHARED
template <class T>
class SharedPtr {
 public:
  // Fields
  T* ptr_ = nullptr;
  Counter* counter_ = nullptr;

  // Constructors
  SharedPtr() = default;

  explicit SharedPtr(T* ptr) : ptr_(ptr) {
    if (ptr) {
      counter_ = new Counter();
      counter_->AddStrong();
    }
  }

  SharedPtr(const WeakPtr<T>& weak) : ptr_(weak.ptr_), counter_(weak.counter_) {  // NOLINT
    if (counter_ && counter_->GetStrong() > 0) {
      counter_->AddStrong();
    } else {
      throw BadWeakPtr{};
    }
  }

  // Copy constructor
  SharedPtr(const SharedPtr<T>& shared) : ptr_(shared.ptr_), counter_(shared.counter_) {
    if (counter_) {
      counter_->AddStrong();
    }
  }

  // Move constructor
  SharedPtr(SharedPtr<T>&& shared) noexcept : ptr_(shared.ptr_), counter_(shared.counter_) {
    shared.ptr_ = nullptr;
    shared.counter_ = nullptr;
  }

  // Destructor
  ~SharedPtr() {
    Release();
  }

  // Copy assign
  SharedPtr<T>& operator=(const SharedPtr<T>& shared) {
    if (this == &shared) {
      return *this;
    }

    Release();
    ptr_ = shared.ptr_;
    counter_ = shared.counter_;
    if (counter_) {
      counter_->AddStrong();
    }

    return *this;
  }

  // Move assign
  SharedPtr<T>& operator=(SharedPtr<T>&& shared) noexcept {
    if (this == &shared) {
      return *this;
    }

    Release();
    ptr_ = shared.ptr_;
    counter_ = shared.counter_;
    shared.ptr_ = nullptr;
    shared.counter_ = nullptr;

    return *this;
  }

  // Methods
  void Reset(T* ptr = nullptr) {
    if (ptr_ == ptr) {
      return;
    }

    Release();
    ptr_ = ptr;
    if (ptr) {
      counter_ = new Counter();
      counter_->AddStrong();
    } else {
      counter_ = nullptr;
    }
  }

  void Swap(SharedPtr& shared) {
    std::swap(ptr_, shared.ptr_);
    std::swap(counter_, shared.counter_);
  }

  T* Get() const {
    return ptr_;
  }

  size_t UseCount() const {
    return counter_ ? counter_->GetStrong() : 0;
  }

  // Operators
  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

 private:
  void Release() {
    if (counter_) {
      counter_->RmStrong();
      if (counter_->EmptyStrong()) {
        if (counter_->EmptyWeak()) {
          delete counter_;
        }
        delete ptr_;
      }
      ptr_ = nullptr;
      counter_ = nullptr;
    }
  }
};

// WEAK
template <class T>
class WeakPtr {
 public:
  T* ptr_ = nullptr;
  Counter* counter_ = nullptr;

  // Constructors
  WeakPtr() = default;

  WeakPtr(const SharedPtr<T>& shared) : ptr_(shared.ptr_), counter_(shared.counter_) {  // NOLINT
    if (counter_) {
      counter_->AddWeak();
    }
  }

  // Copy constructor
  WeakPtr(const WeakPtr<T>& weak) : ptr_(weak.ptr_), counter_(weak.counter_) {
    if (counter_) {
      counter_->AddWeak();
    }
  }

  // Move constructor
  WeakPtr(WeakPtr<T>&& weak) noexcept : ptr_(weak.ptr_), counter_(weak.counter_) {
    weak.ptr_ = nullptr;
    weak.counter_ = nullptr;
  }

  // Destructor
  ~WeakPtr() {
    Release();
  }

  // Copy assign
  WeakPtr<T>& operator=(const WeakPtr<T>& weak) {
    if (this == &weak) {
      return *this;
    }

    Release();
    ptr_ = weak.ptr_;
    counter_ = weak.counter_;
    counter_->AddWeak();

    return *this;
  }

  // Move assign
  WeakPtr<T>& operator=(WeakPtr<T>&& weak) noexcept {
    if (this == &weak) {
      return *this;
    }

    Release();
    ptr_ = weak.ptr_;
    counter_ = weak.counter_;
    weak.ptr_ = nullptr;
    weak.counter_ = nullptr;

    return *this;
  }

  // Methods
  void Swap(WeakPtr<T>& weak) {
    std::swap(ptr_, weak.ptr_);
    std::swap(counter_, weak.counter_);
  }

  void Reset() {
    Release();
    ptr_ = nullptr;
    counter_ = nullptr;
  }

  size_t UseCount() const {
    return counter_ ? counter_->GetStrong() : 0;
  }

  bool Expired() const {
    return counter_ == nullptr || counter_->EmptyStrong();
  }

  SharedPtr<T> Lock() const {
    if (Expired()) {
      return SharedPtr<T>();
    }
    return SharedPtr<T>(*this);
  }

 private:
  void Release() {
    if (counter_) {
      counter_->RmWeak();
      if (counter_->EmptyStrong() && counter_->EmptyWeak()) {
        delete counter_;
      }
      counter_ = nullptr;
    }
  }
};

// MakeShared
template <class T, class... Args>
std::shared_ptr<T> MakeShared(Args&&... args) {
  T* ptr = new T(std::forward<Args>(args)...);
  return std::shared_ptr<T>(ptr);
}

#endif  // SHARED_PTR_H_