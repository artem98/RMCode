#include <memory>
#include "decoder.h"

bit_array RM_decoder::decode (bit_array &code)
{
  if (code.size () != m_n)
    return {};
  bit_array decoded_word;
  decoded_word.assign (m_k, bit (0));
  calculate_all_coefs (code.data (), m_n, m_m, m_k, m_r, decoded_word.data ());
  return decoded_word;
}

void clear_mask (bit* mask, size_t size)
{
  for (size_t i = 0; i < size; i++)
    mask[i].set (0);
}

void set_mask_from_line (bit* mask, size_t line, size_t m)
{
  for (size_t i = 0; i < m; i++)
    {
      mask[i].set ((line >> i) & 1);
    }
}

void calculate_all_coefs (bit* table, // will be modified during calculations
                         const size_t table_len,
                         const size_t m, // vars count
                         const size_t k, // coefs count
                         const size_t r, // polynome deg
                         //Result
                         bit* coefs)
{
  size_t curr_max_deg = r;
  std::unique_ptr<bit[]> vars_mask (new bit[m]);
  size_t whole_matrix_size = 1 << m;
  size_t coef_iter = k - 1;

  for (;; curr_max_deg--)
    {
      coef_iter = k - 1;
      // Dumb way: go through all coefs and choose only those whose deg = curr_max_deg
      for (size_t coef_line = whole_matrix_size - 1; ; coef_line--)
        {
          if (module (coef_line, m) != curr_max_deg)
            {
              if (coef_line == 0)
                break;
              if (module (coef_line, m) <= r)
                coef_iter--;
              continue;
            }

          set_mask_from_line (vars_mask.get (), coef_line, m);
          bit coef = calculate_coefficient (vars_mask.get (), m, curr_max_deg,
                                            table, table_len);
          coefs[coef_iter] = coef;
          if (coef.get ())
            change_func_table (vars_mask.get (), m, table, table_len);
          coef_iter--;
          clear_mask (vars_mask.get (), m);

          if (coef_line == 0)
            break;
        }

      if (curr_max_deg == 0)
        break;
    }
}

bit calculate_coefficient (const bit* vars_mask, //1 - if x_i is in excluded monome, 0 - otherwise
                           const size_t vars_len,
                           const size_t free_vars_num, // count of variables in monome (for which 1 is set in mask)
                           const bit* table,
                           const size_t table_len)
{
  size_t other_vals_count = vars_len - free_vars_num;
  size_t block_count = 1 << other_vals_count;
  std::unique_ptr<bit[]> blocks_values (new bit[block_count]);
  std::unique_ptr<bit[]> vars_values_workspace (new bit[vars_len]);
  size_t count_0 = 0;
  size_t count_1 = 0;

  for (size_t block = 0; block < block_count; block++)
    {

      // set other vars values in vars_values_workspace
      for (size_t var_index = 0; var_index < vars_len; var_index++)
        {
          if (vars_mask[var_index].get ())
            continue;

          vars_values_workspace[var_index].set ((block >> var_index) & 1);
        }
      if (block_sum_result (vars_mask, vars_len, free_vars_num,
                            vars_values_workspace.get (), table, table_len).get ())
        count_1++;
      else
        count_0++;
//      blocks_values[block] = block_sum_result (vars_mask, vars_len, free_vars_num,
//                                               vars_values_workspace.get (), table, table_len);
    }

  return bit (count_1 > count_0);//vote (blocks_values.get (), block_count);
}

bit block_sum_result (const bit* vars_mask, //1 - if x_i is in excluded monome, 0 - otherwise
                      const size_t vars_len,
                      const size_t free_vars_num, // count of variables in monome (for which 1 is set in mask)
                      bit* other_vars_values, // values of other variables (for which 0 is set in mask)
                      const bit* table,
                      const size_t table_len)
{
  size_t block_height = 1 << free_vars_num;
  bit sum (0);

  for (size_t free_vars_values = 0; free_vars_values < block_height; free_vars_values++)
    {
      // put free vars values in other_vars_values
      size_t free_var_iter = 0;
      for (size_t var_index = 0; var_index < vars_len; var_index++)
        {
          if (!vars_mask[var_index].get ())
            continue;

          other_vars_values[var_index].set ((free_vars_values >> free_var_iter) & 1);
          free_var_iter++;
        }

      sum = sum + get_table_value_for_variables (table, table_len, other_vars_values, vars_len);
    }

  return sum;
}

// f' = f - a * x_i1 * ... * x_is
// a is supposed to be 1
// x_i1 ... x_is is stored as:
// 0, 0, 1, 0, 1, ... 1, 0
// ......i1....i2.....is..
void change_func_table (const bit* vars_mask,
                        const size_t vars_len,
                        bit* table,
                        const size_t table_len)
{
  auto are_all_chosen_variable_true = [&] (size_t line) ->bool {
    for (size_t ind = 0; ind < vars_len; ind++)
      {
        if (!(!vars_mask[ind].get () || ((line >> ind) & 1))) // if var is chosen it must be 1
          return false;
      }
    return true;
  };

  for (size_t line = 0; line < table_len; line++)
    {
      if (are_all_chosen_variable_true (line))
        table[line].inverse ();
    }
}

bit get_table_value_for_variables (const bit* table,
                                   const size_t /*table_len*/,
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
