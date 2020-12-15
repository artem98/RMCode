#include <cstdio>
#include <cstdlib>

#include "matrix.h"
#include "bit.h"

int main (int argc, char *argv[])
{
  (void) argc;
  (void) argv;

  dence_matrix<bit> A (5, 5);
  for (size_t i = 0; i < 5; i++)
    A.at(i, i) = bit (true);

  A.print ();
  return 0;
}
