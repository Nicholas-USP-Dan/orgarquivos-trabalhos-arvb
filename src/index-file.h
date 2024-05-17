#ifndef INDEX_FILE_H
#define INDEX_FILE_H

#include "adts/dyn-array.h"

DYN_ARRAY* generate_index(FILE *data_fptr);

int write_index(DYN_ARRAY **index_arr, FILE *index_fptr);

DYN_ARRAY* load_index(FILE *index_fptr);

#endif