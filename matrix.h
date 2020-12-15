#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>

template <typename T>
class dence_matrix
{
public:
  dence_matrix (size_t nrows, size_t ncols)
  {
    m_nrows = nrows;
    m_ncols = ncols;
    m_data.resize (m_nrows * m_ncols);
  }

  T& at (size_t i, size_t j) { return m_data[i * m_ncols + j]; }
  void fill (const T& value) { std::fill (m_data.begin(), m_data.end(), value); }

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

#endif//MATRIX_H
