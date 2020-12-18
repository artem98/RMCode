#include "decoder.h"

bit get_table_value_for_variables (const bit* table,
                                   const size_t table_len,
                                   const bit* vars, // from x_1 to x_n
                                   const size_t vars_len)
{
  size_t index = 0; // convert variables' values to index in table

  for (size_t i = 0; i < vars_len; i++)
    {
      index += (vars[i].getInt () << i);
    }

  return table[index];
}


