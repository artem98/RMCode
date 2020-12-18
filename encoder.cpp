#include "encoder.h"
#include <random>

void RM_encoder::fill_generator_matrix ()
{
  dence_matrix<bit> &M = m_generator_mtx;
  M.set (0, 0, bit (1));
  M.set (0, 1, bit (1));
  M.set (1, 0, bit (0));
  M.set (1, 1, bit (1));
  for (size_t sz = 2; sz < m_n; sz *= 2)
    {
      for (size_t row = 0; row < std::min (sz, m_k); row++)
        {
          for (size_t col = sz; col < sz * 2; col++)
            {
              M.set (row, col, M.at (row, col - sz));
            }
        }
      for (size_t row = sz; row < std::min (sz * 2, m_k); row++)
        {
          for (size_t col = sz; col < sz * 2; col++)
            {
              M.set (row, col, M.at (row - sz, col - sz));
            }
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
      for (size_t row = 0; row <= std::min (col, m_k); row++)
        res[col] = res[col] + word[row] * m_generator_mtx.at (row, col);
    }
  return res;
}


bit get_rand_bit ()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 1);
  return bit (distrib(gen));
}

bit_array get_rand_bits (size_t k)
{
  bit_array word;
  for (size_t i = 0; i < k; i++)
    word.push_back (get_rand_bit ());
  return word;
}

void print_bits (bit_array bits)
{
  for (const bit &b : bits)
    std::cout << b;
  std::cout << std::endl;
}
