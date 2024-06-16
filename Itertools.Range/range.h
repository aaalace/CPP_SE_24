#define REVERSE_RANGE_IMPLEMENTED

#ifndef ITERTOOLS_RANGE__RANGE_H_
#define ITERTOOLS_RANGE__RANGE_H_

#include "iterator.h"
#include "reverseIterator.h"

class Range {
 public:
  Range();
  explicit Range(int end);
  Range(int start, int end, int step = 1);

  Iterator begin() const;          // NOLINT
  Iterator end() const;            // NOLINT
  ReverseIterator rbegin() const;  // NOLINT
  ReverseIterator rend() const;    // NOLINT

  int Size() const;
  void FixBorders();

 private:
  int start_;
  int end_;
  int step_;
};

#endif  // ITERTOOLS_RANGE__RANGE_H_
