#include "decoder.h"

// f' = f - a * x_i1 * ... * x_is
// a is supposed to be 1
// x_i1 ... x_is is stored as:
// 0, 0, 1, 0, 1, ... 1, 0
// ......i1....i2.....is..
void change_func_table (const bit* vars,
                        const size_t vars_len,
                        bit* table,
                        const size_t table_len)
{
  auto is_any_chosen_variable_true = [&] (size_t line) ->bool {
    for (size_t ind = 0; ind < vars_len; ind++)
      {
        if (vars[ind].get () && ((line >> ind) & 1))
          return 1;
      }
    return false;
  };

  for (size_t line = 0; line < table_len; line++)
    {
      if (is_any_chosen_variable_true (line))
        table[line].inverse ();
    }
}

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

bit vote (const bit* values_for_vote, const size_t len)
{
  size_t zero_count = 0;
  size_t one_count = 0;

  for (size_t ind = 0; ind < len; ind++)
    {
      if (values_for_vote[ind].get ())
        one_count++;
      else
        zero_count++;
    }
  if (one_count > zero_count)
    return bit (true);
  return bit (false);
}
