#ifndef ITERTOOLS_RANGE__ITERATOR_H_
#define ITERTOOLS_RANGE__ITERATOR_H_

class Iterator {
 public:
  Iterator(int value, int step);
  // ПРАВИЛО ПЯТИ

  int operator*() const;
  Iterator& operator++();

  bool operator==(Iterator other) const;
  bool operator!=(Iterator other) const;

 private:
  int value_;
  int step_;
};

#endif  // ITERTOOLS_RANGE__ITERATOR_H_
