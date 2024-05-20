#ifndef INDEX_FILE_H
#define INDEX_FILE_H

#include <stdio.h>

#include "adts/dyn-array.h"

int64_t get_index(const void *index_reg);

DYN_ARRAY* generate_index(FILE *data_fptr);

int write_index(DYN_ARRAY **index_arr, FILE *index_fptr);

DYN_ARRAY* load_index(FILE *index_fptr);

#endif