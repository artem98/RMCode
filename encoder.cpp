#include "encoder.h"
#include <random>

void RM_encoder::fill_generator_matrix ()
{
  dence_matrix<bit> full_matrix (m_n, m_n);
  full_matrix.set (0, 0, bit (1));
  for (size_t sz = 1; sz < m_n; sz *= 2)
    {
      for (size_t row = 0; row < std::min (sz, m_n); row++)
        {
          for (size_t col = sz; col < std::min (sz * 2, m_n); col++)
            {
              full_matrix.set (row, col, full_matrix.at (row, col - sz));
            }
        }
      for (size_t row = sz; row < std::min (sz * 2, m_n); row++)
        {
          for (size_t col = sz; col < std::min (sz * 2, m_n); col++)
            {
              full_matrix.set (row, col, full_matrix.at (row - sz, col - sz));
            }
        }
    }
  //full_matrix.print();

  dence_matrix<bit> &M = m_generator_mtx;
  size_t trunc_row = 0;
  for (unsigned int row = 0; row < m_n; row++)
    {
      if (trunc_row > m_k)
        {
          printf ("\ntrunc_row: %lu, k: %lu, row: %u\n", trunc_row, m_k, row);
          STOP ("trunc_row is out of bounds");
        }
      if (module (get_bits(row)) > m_r)
        continue;
      for (size_t col = 0; col < m_n; col++)
        M.set (trunc_row, col, full_matrix.at (row, col));
      trunc_row++;
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
      for (size_t row = 0; row < std::min (col + 1, m_k); row++)
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

bit_array get_bits (unsigned int x)
{
  bit_array bits;
  unsigned int i, bit, len = sizeof(unsigned int)*8;
  for (i=0;i<len;i++)
    {
      bit = x>>(len-i-1)&1;
      bits.push_back (bit == 1);
    }
  return bits;
}

size_t module (bit_array bits)
{
  size_t cnt = 0;
  for (bit &b : bits)
    cnt += b.getInt();
  return cnt;
}

int get_rand_int (int min, int max)
{
  int rand_int = std::rand();
  return min + (rand_int % max);
}

bit_array & set_noise (bit_array &word, int noise_num)
{
  std::srand(time(nullptr));
  if (int(word.size()) < noise_num)
  {
    printf("Warning! word size = %d less than noise size = %d", int(word.size()), noise_num);
    noise_num = int(word.size());
  }

  for (int noise_i = 0; noise_i < noise_num; noise_i ++)
    {
      int random_position = get_rand_int(0, int(word.size()));
      word[random_position].inverse();
    }
  return word;
}
