#ifndef _S21_MATRIX_OOP_H_
#define _S21_MATRIX_OOP_H_

#include <cstring>
#include <iostream>
#include <memory>
#include <new>

class S21Matrix {
 private:
  int _rows, _cols;
  double** p_;

 public:
  // accessors and mutators
  /// @brief Получает количество строк в матрице.
  /// @return Количество строк.
  int getRows() const;

  /// @brief Получает количество столбцов в матрице.
  /// @return Количество столбцов.
  int getCols() const;

  /// @brief Устанавливает новое количество строк в матрице.
  /// @param newRows Новое количество строк.
  /// @throw std::invalid_argument Если количество строк <= 0.
  void setRows(int newRows);

  /// @brief Устанавливает новое количество столбцов в матрице.
  /// @param newCols Новое количество столбцов.
  /// @throw std::invalid_argument Если количество столбцов <= 0.
  void setCols(int newCols);

  /// @brief Конструктор по умолчанию, создающий матрицу размером 1x1.
  S21Matrix();

  /// @brief Конструктор, создающий матрицу заданного размера.
  /// @param rows Количество строк.
  /// @param cols Количество столбцов.
  /// @throw std::invalid_argument Если размеры <= 0.
  S21Matrix(int rows, int cols);

  /// @brief Конструктор копирования.
  /// @param other Константная ссылка на копируемую матрицу.
  S21Matrix(const S21Matrix& other);

  /// @brief Конструктор перемещения.
  /// @param o R-value ссылка на перемещаемый объект.
  S21Matrix(S21Matrix&& o) noexcept;

  /// @brief Деструктор, освобождающий память.
  ~S21Matrix();

  /// @brief Оператор копирующего присваивания.
  /// @param o Константная ссылка на присваиваемую матрицу.
  /// @return Ссылка на текущий объект.
  S21Matrix& operator=(const S21Matrix& o);

  /// @brief Оператор перемещающего присваивания.
  /// @param other R-value ссылка на присваиваемую матрицу.
  /// @return Ссылка на текущий объект.
  S21Matrix& operator=(S21Matrix&& other) noexcept;

  /// @brief Проверяет равенство двух матриц.
  /// @param o Константная ссылка на сравниваемую матрицу.
  /// @return true, если матрицы равны, иначе false.
  bool operator==(const S21Matrix& o) const;

  /// @brief Оператор доступа к элементу матрицы.
  /// @param row Индекс строки.
  /// @param col Индекс столбца.
  /// @return Ссылка на элемент матрицы.
  /// @throw std::out_of_range Если индексы выходят за границы.
  double& operator()(int row, int col);

  /// @brief Оператор доступа к элементу матрицы (константный).
  /// @param row Индекс строки.
  /// @param col Индекс столбца.
  /// @return Константная ссылка на элемент матрицы.
  /// @throw std::out_of_range Если индексы выходят за границы.
  const double& operator()(int row, int col) const;

  /// @brief Оператор сложения с другой матрицей.
  /// @param o Константная ссылка на вторую матрицу.
  /// @return Новая матрица, являющаяся суммой.
  S21Matrix operator+(const S21Matrix& o) const;

  /// @brief Оператор сложения с присваиванием.
  /// @param o Константная ссылка на вторую матрицу.
  /// @return Ссылка на текущую матрицу.
  S21Matrix& operator+=(const S21Matrix& o);

  /// @brief Оператор вычитания другой матрицы.
  /// @param o Константная ссылка на вторую матрицу.
  /// @return Новая матрица, являющаяся разностью.
  S21Matrix operator-(const S21Matrix& o) const;

  /// @brief Оператор вычитания с присваиванием.
  /// @param o Константная ссылка на вторую матрицу.
  /// @return Ссылка на текущую матрицу.
  S21Matrix& operator-=(const S21Matrix& o);

  /// @brief Оператор умножения двух матриц.
  /// @param o Константная ссылка на вторую матрицу.
  /// @return Новая матрица, являющаяся произведением.
  /// @throw std::logic_error Если размеры несовместимы для умножения.
  S21Matrix operator*(const S21Matrix& o) const;

  /// @brief Оператор умножения двух матриц с присваиванием.
  /// @param o Константная ссылка на вторую матрицу.
  /// @return Ссылка на текущую матрицу.
  /// @throw std::logic_error Если размеры несовместимы для умножения.
  S21Matrix& operator*=(const S21Matrix& o);

  /// @brief Оператор умножения матрицы на число.
  /// @param num Число, на которое умножается матрица.
  /// @return Ссылка на текущую матрицу.
  S21Matrix& operator*=(double num);

  /// @brief Умножение числа на матрицу.
  /// @param num Число.
  /// @param m Матрица.
  /// @return Новая матрица.
  friend S21Matrix operator*(double num, const S21Matrix& m);

  /// @brief Умножение матрицы на число.
  /// @param m Матрица.
  /// @param num Число.
  /// @return Новая матрица.
  friend S21Matrix operator*(const S21Matrix& m, double num);

  /// @brief Проверяет, равны ли две матрицы.
  /// @param other Константная ссылка на матрицу.
  /// @return true, если равны, иначе false.
  bool EqMatrix(const S21Matrix& other) const;

  /// @brief Прибавляет к текущей матрице другую.
  /// @param o Константная ссылка на вторую матрицу.
  /// @throw std::logic_error Если размерности не совпадают.
  void SumMatrix(const S21Matrix& o);

  /// @brief Вычитает из текущей матрицы другую.
  /// @param other Константная ссылка на вторую матрицу.
  /// @throw std::logic_error Если размерности не совпадают.
  void SubMatrix(const S21Matrix& other);

  /// @brief Умножает матрицу на число.
  /// @param num Число, на которое умножается матрица.
  void MulNumber(double num);

  /// @brief Умножает текущую матрицу на другую.
  /// @param other Константная ссылка на вторую матрицу.
  /// @throw std::logic_error Если размерности несовместимы.
  void MulMatrix(const S21Matrix& other);

  /// @brief Создает новую транспонированную матрицу из текущей и возвращает ее.
  /// @return Транспонированная матрица.
  S21Matrix Transpose() const;

  /// @brief Вычисляет матрицу алгебраических дополнений.
  /// @return Новая матрица алгебраических дополнений.
  /// @throw std::invalid_argument Если матрица не квадратная.
  S21Matrix CalcComplements() const;

  /// @brief Вычисляет определитель матрицы.
  /// @return Значение определителя.
  /// @throw std::invalid_argument Если матрица не квадратная.
  double Determinant() const;

  /// @brief Вычисляет обратную матрицу.
  /// @return Обратная матрица.
  /// @throw std::invalid_argument Если определитель равен нулю.
  S21Matrix InverseMatrix() const;

 private:
  /// @brief Освобождает память, выделенную для матрицы.
  void cleanup();

  /// @brief Проверяет, имеют ли две матрицы одинаковые размеры.
  /// @param other Константная ссылка на вторую матрицу.
  /// @return true, если размеры совпадают, иначе false.
  inline bool CheckSameSize(const S21Matrix& other) const;

  /// @brief Проверяет, корректна ли матрица.
  /// @throw std::invalid_argument Если матрица некорректна.
  void IsMatrixValid() const;

  /// @brief Проверяет, является ли матрица квадратной.
  /// @throw std::invalid_argument Если матрица не квадратная.
  void IsMatrixSquare() const;

  /// @brief Меняет содержимое текущей матрицы с другой.
  /// @param other Матрица для обмена данными.
  /// @throw noexcept Гарантирует, что не выбросит исключение.
  void Swap(S21Matrix& other) noexcept;

  /// @brief Удаляет указанную строку и столбец из матрицы.
  /// @param row_index Индекс удаляемой строки.
  /// @param column_index Индекс удаляемого столбца.
  /// @return Новая матрица без указанной строки и столбца.
  S21Matrix RemoveRowCol(int row_index, int column_index) const;

  /// @brief Вычисляет минор элемента матрицы.
  /// @param row Индекс строки.
  /// @param col Индекс столбца.
  /// @return Значение минора.
  double Minor(int row, int col) const;
};

#endif  // _S21_MATRIX_OOP_H_
