#include <cmath>

#include "s21_matrix_oop.h"

S21Matrix::S21Matrix(int rows, int cols) : _rows(rows), _cols(cols) {
  if (_rows <= 0 || _cols <= 0) {
    throw std::invalid_argument("Invalid matrix dimensions");
  }
  try {
    p_ = new double*[_rows];
    p_[0] = new double[_rows * _cols]();
    for (int i = 1; i < _rows; ++i) {
      p_[i] = p_[0] + i * _cols;
    }

  } catch (const std::bad_alloc&) {
    cleanup();
    throw;
  }
}

S21Matrix::S21Matrix() : S21Matrix(1, 1) {}

S21Matrix::~S21Matrix() { cleanup(); }

int S21Matrix::getRows() const { return _rows; }
int S21Matrix::getCols() const { return _cols; }

void S21Matrix::setRows(int newRows) {
  if (newRows <= 0) {
    throw std::invalid_argument("The number of rows must be greater than zero");
  }

  S21Matrix temp(newRows, _cols);
  int minRows = std::min(newRows, _rows);

  for (int i = 0; i < minRows; ++i) {
    std::copy(p_[i], p_[i] + _cols, temp.p_[i]);
  }
  *this = std::move(temp);
}

void S21Matrix::setCols(int newCols) {
  if (newCols <= 0) {
    throw std::invalid_argument(
        "The number of columns must be greater than zero");
  }

  S21Matrix temp(_rows, newCols);
  int minCols = std::min(newCols, _cols);

  for (int i = 0; i < _rows; ++i) {
    std::copy(p_[i], p_[i] + minCols, temp.p_[i]);
  }
  *this = std::move(temp);
}

S21Matrix::S21Matrix(const S21Matrix& o) : S21Matrix(o._rows, o._cols) {
  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < _cols; ++j) {
      p_[i][j] = o.p_[i][j];
    }
  }
}

S21Matrix::S21Matrix(S21Matrix&& o) noexcept
    : _rows(o._rows), _cols(o._cols), p_(o.p_) {
  o._rows = 0;
  o._cols = 0;
  o.p_ = nullptr;
}

bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  IsMatrixValid();
  other.IsMatrixValid();
  if ((this->CheckSameSize(other)) == false) {
    return false;
  }
  const double kPrecision = 0.000001;
  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < _cols; ++j) {
      if (std::fabs(p_[i][j] - other.p_[i][j]) > kPrecision) {
        return false;
      }
    }
  }
  return true;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  IsMatrixValid();
  other.IsMatrixValid();
  if ((CheckSameSize(other)) == false) {
    throw std::logic_error("different matrix dimensions");
  }
  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < _cols; ++j) {
      p_[i][j] += other.p_[i][j];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  IsMatrixValid();
  other.IsMatrixValid();
  if (!(CheckSameSize(other))) {
    throw std::logic_error("different matrix dimensions");
  }
  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < _cols; ++j) {
      p_[i][j] -= other.p_[i][j];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  IsMatrixValid();

  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < _cols; ++j) {
      p_[i][j] *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  IsMatrixValid();
  other.IsMatrixValid();

  if (_cols != other._rows) {
    throw std::logic_error(
        "Matrix dimensions are incompatable for multiplication");
  }
  S21Matrix temp(_rows, other._cols);
  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < other._cols; ++j) {
      temp.p_[i][j] = 0;
      for (auto k = 0; k < _cols; ++k) {
        temp.p_[i][j] += p_[i][k] * other.p_[k][j];
      }
    }
  }
  *this = std::move(temp);
}

S21Matrix S21Matrix::Transpose() const {
  IsMatrixValid();
  S21Matrix transposed(_cols, _rows);
  for (auto i = 0; i < transposed._rows; ++i) {
    for (auto j = 0; j < transposed._cols; ++j) {
      transposed.p_[i][j] = p_[j][i];
    }
  }
  return transposed;
}

S21Matrix S21Matrix::CalcComplements() const {
  IsMatrixValid();
  IsMatrixSquare();

  S21Matrix result(_rows, _cols);
  for (auto i = 0; i < _rows; ++i) {
    for (auto j = 0; j < _cols; ++j) {
      result.p_[i][j] = this->Minor(i, j) * pow(-1, i + j);
    }
  }
  return result;
}

double S21Matrix::Determinant() const {
  IsMatrixValid();
  IsMatrixSquare();

  double result = 0;
  if (_rows == 1) {
    result = p_[0][0];
  } else if (_rows == 2) {
    result = p_[0][0] * p_[1][1] - p_[0][1] * p_[1][0];
  } else {
    int sign = 1;
    for (auto i = 0; i < _rows; ++i) {
      result += this->Minor(0, i) * p_[0][i] * sign;
      sign *= -1;
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() const {
  IsMatrixValid();
  IsMatrixSquare();

  double determinant = Determinant();
  if (determinant == 0) {
    throw std::invalid_argument(
        "Matrix determinant = 0, inverse does not exist");
  }
  S21Matrix result(_cols, _rows);

  if (_cols == 1 && _rows == 1) {
    result.p_[0][0] = 1 / p_[0][0];
  } else {
    result = CalcComplements();
    result = result.Transpose();
    result *= (1 / determinant);
  }
  return result;
}

/* operators overloads */

S21Matrix& S21Matrix::operator+=(const S21Matrix& o) {
  this->SumMatrix(o);
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& o) const {
  S21Matrix result(*this);
  result.SumMatrix(o);
  return result;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& o) {
  this->SubMatrix(o);
  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix& o) const {
  S21Matrix result(*this);
  result.SubMatrix(o);
  return result;
}

bool S21Matrix::operator==(const S21Matrix& o) const {
  return this->EqMatrix(o);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& o) {
  if (this != &o) {
    S21Matrix temp(o);
    Swap(temp);
  }
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this != &other) {
    S21Matrix temp{std::move(other)};
    Swap(temp);
  }
  return *this;
}

double& S21Matrix::operator()(int row, int col) {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
    throw std::out_of_range("Row or column indices exceed matrix dimensions");
  }
  return p_[row][col];
}

const double& S21Matrix::operator()(int row, int col) const {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
    throw std::out_of_range("Row or column indices exceed matrix dimensions");
  }
  return p_[row][col];
}

S21Matrix S21Matrix::operator*(const S21Matrix& o) const {
  S21Matrix result(*this);
  result.MulMatrix(o);
  return result;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& o) {
  MulMatrix(o);
  return *this;
}

S21Matrix& S21Matrix::operator*=(double num) {
  MulNumber(num);
  return *this;
}

S21Matrix operator*(double num, const S21Matrix& m) {
  S21Matrix result(m);
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(const S21Matrix& m, double num) {
  S21Matrix result(m);
  result.MulNumber(num);
  return result;
}

/* private */

void S21Matrix::cleanup() {
  if (p_) {
    delete[] p_[0];
    delete[] p_;
    p_ = nullptr;
  }
}

inline bool S21Matrix::CheckSameSize(const S21Matrix& other) const {
  return _rows == other._rows && _cols == other._cols;
}

void S21Matrix::IsMatrixValid() const {
  if (p_ == nullptr || _rows <= 0 || _cols <= 0) {
    throw std::invalid_argument("Invalid matrix");
  }
}

void S21Matrix::IsMatrixSquare() const {
  if (_rows != _cols) {
    throw std::invalid_argument("Invalid matrix");
  }
}

void S21Matrix::Swap(S21Matrix& other) noexcept {
  std::swap(_rows, other._rows);
  std::swap(_cols, other._cols);
  std::swap(p_, other.p_);
}

S21Matrix S21Matrix::RemoveRowCol(int row_index, int column_index) const {
  S21Matrix result(_rows - 1, _cols - 1);

  int new_row = 0, new_column = 0;
  for (auto i = 0; i < _rows; ++i) {
    if (i == row_index) continue;
    new_column = 0;
    for (auto j = 0; j < _cols; ++j) {
      if (j == column_index) continue;

      result.p_[new_row][new_column] = p_[i][j];
      new_column++;
    }
    new_row++;
  }
  return result;
}

double S21Matrix::Minor(int row, int col) const {
  double minor = 0;
  if (_rows == 1 && _cols == 1) {
    minor = 1;
  } else {
    minor = RemoveRowCol(row, col).Determinant();
  }
  return minor;
}
