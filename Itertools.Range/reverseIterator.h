#ifndef ITERTOOLS_RANGE__REVERSEITERATOR_H_
#define ITERTOOLS_RANGE__REVERSEITERATOR_H_

#include "iterator.h"

class ReverseIterator {
 public:
  ReverseIterator(int, int);

  int operator*() const;
  ReverseIterator& operator++();
  bool operator==(ReverseIterator other) const;
  bool operator!=(ReverseIterator other) const;

 private:
  int value_;
  int step_;
};

#endif  // ITERTOOLS_RANGE__REVERSEITERATOR_H_
