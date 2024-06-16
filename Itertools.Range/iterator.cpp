#include "iterator.h"

Iterator::Iterator(int value, int step) : value_(value), step_(step) {
}

int Iterator::operator*() const {
  return value_;
}

Iterator& Iterator::operator++() {
  value_ += step_;
  return *this;
}

bool Iterator::operator==(Iterator other) const {
  return value_ == other.value_;
}

bool Iterator::operator!=(Iterator other) const {
  return value_ != other.value_;
}