#include "reverseIterator.h"

ReverseIterator::ReverseIterator(int value, int step) : value_(value), step_(-step) {
}

int ReverseIterator::operator*() const {
  return value_;
}

ReverseIterator& ReverseIterator::operator++() {
  value_ += step_;
  return *this;
}

bool ReverseIterator::operator==(ReverseIterator other) const {
  return value_ == other.value_;
}

bool ReverseIterator::operator!=(ReverseIterator other) const {
  return value_ != other.value_;
}
