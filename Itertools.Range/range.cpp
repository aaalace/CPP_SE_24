#include "range.h"

Range::Range() = default;

// Constructors
Range::Range(int end) {
  start_ = 0;
  end_ = end;
  step_ = 1;
  FixBorders();
}

Range::Range(int start, int end, int step) {
  start_ = start;
  end_ = end;
  step_ = step;
  FixBorders();
}

// Iterator getters
Iterator Range::begin() const {
  return {start_, step_};
}

Iterator Range::end() const {
  int ref_end = (end_ - start_) % step_ == 0 ? end_ : start_ + ((end_ - start_ + step_ - 1) / step_) * step_;
  return {ref_end, step_};
}

ReverseIterator Range::rbegin() const {
  int ref_end = (end_ - start_) % step_ == 0 ? end_ : start_ + ((end_ - start_ + step_ - 1) / step_) * step_;
  return {ref_end - step_, step_};
}

ReverseIterator Range::rend() const {
  return {start_ - step_, step_};
}

// Custom
int Range::Size() const {
  return (end_ - start_) / step_;
}

void Range::FixBorders() {
  if ((start_ >= end_ && step_ >= 0) || (end_ >= start_ && step_ <= 0)) {
    end_ = start_;
    step_ = 1;
  }
}