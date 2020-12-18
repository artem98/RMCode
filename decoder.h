#ifndef DECODER_H
#define DECODER_H

#include "bit.h"

bit get_table_value_for_variables (const bit* table,
                                   const size_t table_len,
                                   const bit* vars, // from x_1 to x_n
                                   const size_t vars_len);

#endif // DECODER_H
