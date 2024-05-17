#ifndef DYNAMIC_ARR_H
#define DYNAMIC_ARR_H

#include <stdint.h>
#include "../utils/data-file-utils.h"

typedef struct _dyn_sorted_array DYN_SORTED_ARRAY;

DYN_SORTED_ARRAY* initialize_dyn_array();

void clear_dyn_array(DYN_SORTED_ARRAY **array);

INDEX_REG get_dyn_array(int64_t i, DYN_SORTED_ARRAY **array);

int64_t find_pos_dyn_array(int32_t index, DYN_SORTED_ARRAY **array);

int insert_dyn_array(INDEX_REG reg, DYN_SORTED_ARRAY **array);

int remove_dyn_array(int32_t index, DYN_SORTED_ARRAY **array);

int64_t length_dyn_array(DYN_SORTED_ARRAY **array);

#endif