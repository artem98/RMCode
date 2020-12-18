#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "matrix.h"
#include "bit.h"
#include "encoder.h"

int main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printf ("Usage: %s <r> <m>\n", argv[0]);
      return 1;
    }
  int r = atoi (argv[1]);
  int m = atoi (argv[2]);
  printf ("r = %d\n", r);
  printf ("m = %d\n", m);
  if (r < 0 || m < 0 || r >= m)
    {
      printf ("Parameters must satisfy the following relation: 0 < r < m \n");
      return 1;
    }

  RM_encoder encoder (r, m);
  //encoder.print_generator_matrix();

  size_t k = encoder.get_k();
  bit_array word = get_rand_bits (k);
  std::cout << "Word to encode:" << std::endl;
  print_bits (word);

  bit_array code = encoder.encode(word);
  std::cout << "Code:" << std::endl;
  print_bits (code);

  return 0;
}
