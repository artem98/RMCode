#ifndef ENCODER_H
#define ENCODER_H

#include "bit.h"
#include "matrix.h"
#include <memory>
#include <cstdio>
#include <cmath>

inline size_t bcr (int n, int k) // binomial coefficient C_n^k
{
  if (k > n / 2)
    k = n - k;
  if (k == 1)
    return n;
  if (k == 0)
    return 1;
  return bcr (n - 1, k) + bcr (n - 1, k - 1);
}

inline size_t code_dim (int r, int m)
{
  int dim = 0;
  for (int i = 0; i <= r; i++)
    {
      dim += bcr (m, i);
    }
  return dim;
}

class RM_encoder
{
public:
  RM_encoder (int r, int m)
  {
    m_n = pow (2, m);
    printf ("n = %lu\n", m_n);
    m_k = code_dim (r, m);
    printf ("k = %lu\n", m_k);
    m_generator_mtx.init (m_k, m_n);
    fill_generator_matrix ();
  }
  void print_generator_matrix () { m_generator_mtx.print(); }
  bit_array encode (const bit_array &word);

  size_t get_k () { return m_k; }
  size_t get_n () { return m_n; }

private:
  void fill_generator_matrix ();

  dence_matrix<bit> m_generator_mtx;
  size_t m_k = 0;
  size_t m_n = 0;
};

bit get_rand_bit ();
bit_array get_rand_bits (size_t k);
void print_bits (bit_array bits);

#endif//ENCODER_H
