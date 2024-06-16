#ifndef MATRIX_H_
#define MATRIX_H_

#define MATRIX_SQUARE_MATRIX_IMPLEMENTED

#include <ostream>
#include <stdexcept>

class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {
  }
};

class MatrixOutOfRange : public std::out_of_range {
 public:
  explicit MatrixOutOfRange(const std::string& msg) : std::out_of_range(msg) {
  }
};

class MatrixDimensionMismatch : public std::logic_error {
 public:
  MatrixDimensionMismatch() : std::logic_error("Matrix dimensions mismatched") {
  }
};

template <class T, size_t R, size_t C>
class Matrix {
 public:
  T matrix_[R][C];
  size_t RowsNumber() const {
    return R;
  }
  size_t ColumnsNumber() const {
    return C;
  }

  T& operator()(size_t i, size_t j) {
    return matrix_[i][j];
  }

  const T& operator()(size_t i, size_t j) const {
    return matrix_[i][j];
  }

  T& At(size_t i, size_t j) {
    if (i >= R || j >= C) {
      throw MatrixOutOfRange("Matrix index out of range");
    }

    return matrix_[i][j];
  }

  const T& At(size_t i, size_t j) const {
    if (i >= R || j >= C) {
      throw MatrixOutOfRange("Matrix index out of range");
    }

    return matrix_[i][j];
  }

  friend Matrix<T, C, R> GetTransposed(Matrix<T, R, C>& matrix) {
    Matrix<T, C, R> transposed;
    for (size_t i = 0; i < C; ++i) {
      for (size_t j = 0; j < R; ++j) {
        transposed.matrix_[i][j] = matrix.matrix_[j][i];
      }
    }

    return transposed;
  }

  template <size_t OR, size_t OC>
  Matrix<T, R, C> operator+(const Matrix<T, OR, OC>& other_matrix) const {
    if (R != OR || C != OC) {
      throw MatrixDimensionMismatch{};
    }
    Matrix<T, R, C> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        matrix.matrix_[i][j] = this->matrix_[i][j] + other_matrix.matrix_[i][j];
      }
    }

    return matrix;
  }

  template <size_t OR, size_t OC>
  Matrix<T, R, C>& operator+=(const Matrix<T, OR, OC>& other_matrix) {
    if (R != OR || C != OC) {
      throw MatrixDimensionMismatch{};
    }
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        this->matrix_[i][j] += other_matrix.matrix_[i][j];
      }
    }

    return *this;
  }

  template <size_t OR, size_t OC>
  Matrix<T, R, C> operator-(const Matrix<T, OR, OC>& other_matrix) const {
    if (R != OR || C != OC) {
      throw MatrixDimensionMismatch{};
    }
    Matrix<T, R, C> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        matrix.matrix_[i][j] = this->matrix_[i][j] - other_matrix.matrix_[i][j];
      }
    }

    return matrix;
  }

  template <size_t OR, size_t OC>
  Matrix<T, R, C>& operator-=(const Matrix<T, OR, OC>& other_matrix) {
    if (R != OR || C != OC) {
      throw MatrixDimensionMismatch{};
    }
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        this->matrix_[i][j] -= other_matrix.matrix_[i][j];
      }
    }

    return *this;
  }

  template <size_t OR, size_t OC>
  Matrix<T, R, OC> operator*(const Matrix<T, OR, OC>& other_matrix) const {
    if (C != OR) {
      throw MatrixDimensionMismatch{};
    }
    Matrix<T, R, OC> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < OC; ++j) {
        T sum = T();
        for (size_t k = 0; k < C; ++k) {
          sum += this->matrix_[i][k] * other_matrix.matrix_[k][j];
        }
        matrix.matrix_[i][j] = sum;
      }
    }

    return matrix;
  }

  template <size_t OR, size_t OC>
  Matrix<T, R, OC>& operator*=(const Matrix<T, OR, OC>& other_matrix) {
    if (C != OR) {
      throw MatrixDimensionMismatch{};
    }
    Matrix<T, R, OC> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < OC; ++j) {
        T sum = T();
        for (size_t k = 0; k < C; ++k) {
          sum += this->matrix_[i][k] * other_matrix.matrix_[k][j];
        }
        matrix.matrix_[i][j] = sum;
      }
    }
    *this = matrix;

    return *this;
  }

  Matrix<T, R, C> operator*(const T& scalar) const {
    Matrix<T, R, C> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        matrix.matrix_[i][j] = this->matrix_[i][j] * scalar;
      }
    }

    return matrix;
  }

  friend Matrix<T, R, C> operator*(const T& scalar, Matrix<T, R, C> matrix) {
    return matrix * scalar;
  }

  Matrix<T, R, C>& operator*=(const T& scalar) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        this->matrix_[i][j] *= scalar;
      }
    }

    return *this;
  }

  Matrix<T, R, C> operator/(const T& scalar) const {
    Matrix<T, R, C> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        matrix.matrix_[i][j] = this->matrix_[i][j] / scalar;
      }
    }

    return matrix;
  }

  Matrix<T, R, C>& operator/=(const T& scalar) {
    Matrix<T, R, C> matrix;
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        this->matrix_[i][j] /= scalar;
      }
    }

    return *this;
  }

  template <size_t OR, size_t OC>
  bool operator==(const Matrix<T, OR, OC>& matrix) const {
    if (R != OR || C != OC) {
      throw MatrixDimensionMismatch{};
    }
    for (size_t i = 0; i < R; i++) {
      for (size_t j = 0; j < C; j++) {
        if (this->matrix_[i][j] != matrix.matrix_[i][j]) {
          return false;
        }
      }
    }

    return true;
  }

  template <size_t OR, size_t OC>
  bool operator!=(const Matrix<T, OR, OC>& matrix) const {
    return !(*this == matrix);
  }

  friend std::istream& operator>>(std::istream& is, Matrix<T, R, C>& matrix) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        is >> matrix.matrix_[i][j];
      }
    }

    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& matrix) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        if (j != C - 1) {
          os << matrix.matrix_[i][j] << ' ';
        } else {
          os << matrix.matrix_[i][j] << '\n';
        }
      }
    }

    return os;
  }

  // Additional
  void Transpose() {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    for (std::size_t i = 0; i < R; ++i) {
      for (std::size_t j = i + 1; j < C; ++j) {
        std::swap(matrix_[i][j], matrix_[j][i]);
      }
    }
  }

  T Trace() const {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    T trace = T();
    for (std::size_t i = 0; i < R; ++i) {
      trace += matrix_[i][i];
    }
    return trace;
  }

  Matrix<T, R, C> GetInversed() const {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    Matrix<T, R, C> result = *this;
    result.Inverse();
    return result;
  }

  void Inverse() {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    Matrix<T, R, C> id;
    for (std::size_t i = 0; i < R; ++i) {
      id(i, i) = 1;
    }
    if (!Gauss(*this, id)) {
      throw MatrixIsDegenerateError();
    }

    *this = id;
  }

  // Custom
  T DeterminantRecursive(const Matrix<T, R, C>& matrix, std::size_t size) const {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    if (size == 1) {
      return matrix(0, 0);
    }
    if (size == 2) {
      return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
    }
    T det = 0;
    Matrix<T, R, C> sub_matrix;
    for (std::size_t i = 0; i < size; ++i) {
      GetSubMatrix(matrix, sub_matrix, i, size);
      det += (i % 2 == 0 ? 1 : -1) * matrix(0, i) * DeterminantRecursive(sub_matrix, size - 1);
    }

    return det;
  }

  void GetSubMatrix(const Matrix<T, R, C>& matrix, Matrix<T, R, C>& sub_matrix, std::size_t q, std::size_t size) const {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    std::size_t sub_i = 0;
    for (std::size_t i = 1; i < size; ++i) {
      std::size_t sub_j = 0;
      for (std::size_t j = 0; j < size; ++j) {
        if (j == q) {
          continue;
        }
        sub_matrix(sub_i, sub_j) = matrix(i, j);
        ++sub_j;
      }
      ++sub_i;
    }
  }

  bool Gauss(Matrix<T, R, C>& matrix, Matrix<T, R, C>& id) const {
    if (R != C) {
      throw MatrixDimensionMismatch{};
    }
    for (std::size_t i = 0; i < R; ++i) {
      if (matrix(i, i) == 0) {
        std::size_t swap_row = i + 1;
        while (swap_row < R && matrix(swap_row, i) == 0) {
          ++swap_row;
        }
        if (swap_row == R) {
          return false;
        }
        std::swap_ranges(matrix.data[i].begin(), matrix.data[i].end(), matrix.data[swap_row].begin());
        std::swap_ranges(id.data[i].begin(), id.data[i].end(), id.data[swap_row].begin());
      }

      T diag = matrix(i, i);
      for (std::size_t j = 0; j < R; ++j) {
        matrix(i, j) /= diag;
        id(i, j) /= diag;
      }

      for (std::size_t row = 0; row < R; ++row) {
        if (row != i) {
          T factor = matrix(row, i);
          for (std::size_t col = 0; col < C; ++col) {
            matrix(row, col) -= factor * matrix(i, col);
            id(row, col) -= factor * id(i, col);
          }
        }
      }
    }

    return true;
  }
};

template <typename T, std::size_t R, std::size_t C>
void Transpose(Matrix<T, R, C>& matrix) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  for (std::size_t i = 0; i < R; ++i) {
    for (std::size_t j = i + 1; j < C; ++j) {
      std::swap(matrix(i, j), matrix(j, i));
    }
  }
}

template <typename T, std::size_t R, std::size_t C>
T Trace(const Matrix<T, R, C>& matrix) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  T trace = T();
  for (std::size_t i = 0; i < R; ++i) {
    trace += matrix(i, i);
  }
  return trace;
}

template <typename T, std::size_t R, std::size_t C>
T Determinant(const Matrix<T, R, C>& matrix) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  return DeterminantRecursive(matrix, R);
}

template <typename T, std::size_t R, std::size_t C>
Matrix<T, R, C> GetInversed(const Matrix<T, R, C>& matrix) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  Matrix<T, R, C> result = matrix;
  Inverse(result);
  return result;
}

template <typename T, std::size_t R, std::size_t C>
void Inverse(Matrix<T, R, C>& matrix) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  Matrix<T, R, C> id;
  for (std::size_t i = 0; i < R; ++i) {
    id(i, i) = 1;
  }
  if (!Gauss(matrix, id)) {
    throw MatrixIsDegenerateError();
  }
  matrix = id;
}

// Custom
template <typename T, std::size_t R, std::size_t C>
T DeterminantRecursive(const Matrix<T, R, C>& matrix, std::size_t size) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  if (size == 1) {
    return matrix(0, 0);
  }
  if (size == 2) {
    return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
  }

  T det = 0;
  Matrix<T, R, C> submatrix;

  for (std::size_t x = 0; x < size; ++x) {
    GetSubMatrix(matrix, submatrix, x, size);
    det += (x % 2 == 0 ? 1 : -1) * matrix(0, x) * DeterminantRecursive(submatrix, size - 1);
  }

  return det;
}

template <typename T, std::size_t R, std::size_t C>
void GetSubMatrix(const Matrix<T, R, C>& matrix, Matrix<T, R, C>& submatrix, std::size_t q, std::size_t size) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  std::size_t sub_i = 0;
  for (std::size_t i = 1; i < size; ++i) {
    std::size_t sub_j = 0;
    for (std::size_t j = 0; j < size; ++j) {
      if (j == q) {
        continue;
      }
      submatrix(sub_i, sub_j) = matrix(i, j);
      ++sub_j;
    }
    ++sub_i;
  }
}

template <typename T, std::size_t R, std::size_t C>
bool Gauss(Matrix<T, R, C>& matrix, Matrix<T, R, C>& identity) {
  if (R != C) {
    throw MatrixDimensionMismatch{};
  }
  for (std::size_t i = 0; i < R; ++i) {
    if (matrix(i, i) == 0) {
      std::size_t swap_row = i + 1;
      while (swap_row < R && matrix(swap_row, i) == 0) {
        ++swap_row;
      }
      if (swap_row == R) {
        return false;
      }
      for (std::size_t col = 0; col < C; ++col) {
        std::swap(matrix(i, col), matrix(swap_row, col));
        std::swap(identity(i, col), identity(swap_row, col));
      }
    }

    T diag = matrix(i, i);
    for (std::size_t j = 0; j < C; ++j) {
      matrix(i, j) /= diag;
      identity(i, j) /= diag;
    }

    for (std::size_t row = 0; row < R; ++row) {
      if (row != i) {
        T factor = matrix(row, i);
        for (std::size_t col = 0; col < C; ++col) {
          matrix(row, col) -= factor * matrix(i, col);
          identity(row, col) -= factor * identity(i, col);
        }
      }
    }
  }
  return true;
}

#endif  // MATRIX_H_
