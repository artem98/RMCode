#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>

template <typename T>
class dence_matrix
{
public:
  dence_matrix () {}
  dence_matrix (size_t nrows, size_t ncols)
  {
    init (nrows, ncols);
  }
  void init (size_t nrows, size_t ncols)
  {
    m_nrows = nrows;
    m_ncols = ncols;
    m_data.resize (m_nrows * m_ncols);
  }

  void set (size_t i, size_t j, const T& val)
  {
    m_data[i * m_ncols + j] = val;
  }
  T at (size_t i, size_t j)
  {
    return m_data[i * m_ncols + j];
  }

  void print ()
  {
    for (size_t row = 0; row < m_nrows; row++)
      {
        for (size_t col = 0; col < m_ncols; col++)
          {
            std::cout << m_data[row * m_ncols + col] << " ";
          }
        std::cout << std::endl;
      }
  }

private:
  std::vector<T> m_data;
  size_t m_nrows = 0;
  size_t m_ncols = 0;
};

template <typename T>
class square_upper_triangular_matrix
{
public:
  square_upper_triangular_matrix () {}
  square_upper_triangular_matrix (size_t size)
  {
    init (size);
  }
  void init (size_t size)
  {
    m_size = size;
    m_data.resize (size * (size + 1) / 2);
  }

  void set (size_t i, size_t j, const T& val)
  {
    if (i > j)
      return;
    m_data[(i * m_size + j) - (i + 1) * i / 2] = val;
  }
  T at (size_t i, size_t j)
  {
    if (i > j)
      return {};
    return m_data[(i * m_size + j) - (i + 1) * i / 2];
  }

  void print ()
  {
    for (size_t row = 0; row < m_size; row++)
      {
        for (size_t col = 0; col < m_size; col++)
          {
            if (row > col)
              std::cout << "- ";
            else
              std::cout << at (row, col) << " ";
          }
        std::cout << std::endl;
      }
  }

private:
  std::vector<T> m_data;
  size_t m_size = 0;
};

#endif//MATRIX_H
