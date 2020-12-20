#include "bit.h"
#include <random>

bit Not (bit& b)
{
  return bit (!b.get ());
}

int toInt (bit& b)
{
  return b.get () ? 1 : 0;
}

bool operator> (const bit &lhs, const bit &rhs)
{
  return lhs.m_bit && !rhs.m_bit;
}
bool operator<= (const bit &lhs, const bit &rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

bool operator< (const bit &lhs, const bit &rhs)
{
  return !lhs.m_bit && rhs.m_bit;
}
bool operator>= (const bit &lhs, const bit &rhs)
{
  return (lhs > rhs) || (lhs == rhs) ;
}

bool operator== (const bit &lhs, const bit &rhs)
{
  return (lhs.m_bit && rhs.m_bit) || (!lhs.m_bit && !rhs.m_bit);
}
bool operator!= (const bit &lhs, const bit &rhs)
{
  return (lhs.m_bit && !rhs.m_bit) || (!lhs.m_bit && rhs.m_bit);
}

std::ostream& operator<< (std::ostream &out, const bit &b)
{
  out << b.getInt();
  return out;
}

// utils

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

size_t module (size_t n, size_t bound)
{
  size_t res = 0;
  for (size_t i = 0; i < bound; i++, n >>= 1)
    {
      if (n & 1)
        res++;
    }
  return res;
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

size_t diff (const bit_array &bits1, const bit_array &bits2)
{
  size_t size = std::min (bits1.size (), bits2.size ());
  size_t diff = 0;
  for (size_t i = 0; i < size; i++)
    {
      bit d = bits1[i] + bits2[i];
      if (d.get())
        diff++;
    }
  return diff;
}

