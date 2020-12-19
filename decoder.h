#ifndef DECODER_H
#define DECODER_H

#include "bit.h"

void calculate_all_coefs (bit* table, // will be modified during calculations
                         const size_t table_len,
                         const size_t m, // vars count
                         const size_t k, // coefs count
                         const size_t r, // polynome deg
                         //Result
                         bit* coefs);

bit calculate_coefficient (const bit* vars_mask, //1 - if x_i is in excluded monome, 0 - otherwise
                           const size_t vars_len,
                           const size_t free_vars_num, // count of variables in monome (for which 1 is set in mask)
                           const bit* table,
                           const size_t table_len);

bit block_sum_result (const bit* vars_mask, //1 - if x_i is in excluded monome, 0 - otherwise
                      const size_t vars_len,
                      const size_t free_vars_num, // count of variables in monome (for which 1 is set in mask)
                      bit* other_vars_values, // values of other variables (for which 0 is set in mask)
                      const bit* table,
                      const size_t table_len);


// f' = f - a * x_i1 * ... * x_is
// a is supposed to be 1
void change_func_table (const bit* vars,
                        const size_t vars_len,
                        bit* table,
                        const size_t table_len);

bit get_table_value_for_variables (const bit* table,
                                   const size_t table_len,
                                   const bit* vars, // from x_1 to x_n
                                   const size_t vars_len);

bit vote (const bit* values_for_vote, const size_t len);

#endif // DECODER_H
