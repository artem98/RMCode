#ifndef DECODER_H
#define DECODER_H

#include "bit.h"

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
