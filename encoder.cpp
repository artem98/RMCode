#include "encoder.h"

void RM_encoder::fill_generator_matrix ()
{
  square_upper_triangular_matrix<bit> full_matrix (m_n);
  full_matrix.set (0, 0, bit (1));
  for (size_t sz = 1; sz < m_n; sz *= 2)
    {
      size_t ub1 = std::min (sz, m_n);
      size_t ub2 = std::min (sz * 2, m_n);
      for (size_t row = 0; row < ub1; row++)
        {
          size_t lb1 = std::max (row, sz);
          for (size_t col = lb1; col < ub2; col++)
            {
              full_matrix.set (row, col, full_matrix.at (row, col - sz));
            }
        }
      for (size_t row = sz; row < ub2; row++)
        {
          size_t lb1 = std::max (row, sz);
          for (size_t col = lb1; col < ub2; col++)
            {
              full_matrix.set (row, col, full_matrix.at (row - sz, col - sz));
            }
        }
    }
  //full_matrix.print();

  dence_matrix<bit> &M = m_generator_mtx;
  size_t trunc_row = 0;
  for (size_t row = 0; row < m_n; row++)
    {
      if (trunc_row > m_k)
        {
          printf ("\ntrunc_row: %lu, k: %lu, row: %lu\n", trunc_row, m_k, row);
          STOP ("trunc_row is out of bounds");
        }
      if (module (row, sizeof (size_t) * 8) <= m_r)
        {
          for (size_t col = row; col < m_n; col++)
            M.set (trunc_row, col, full_matrix.at (row, col));
          trunc_row++;
        }
    }
}

bit_array RM_encoder::encode (const bit_array &word)
{
  if (word.size () != m_k)
    return {};
  bit_array res;
  res.assign (m_n, bit (0));
  for (size_t col = 0; col < m_n; col++)
    {
      size_t ub = std::min (col + 1, m_k);
      for (size_t row = 0; row < ub; row++)
        res[col] = res[col] + word[row] * m_generator_mtx.at (row, col);
    }
  return res;
}
