#ifndef DYNAMIC_ARR_H
#define DYNAMIC_ARR_H

#include <stdint.h>

#include "../utils/index-types.h"

struct _arr_el {
    INDEX_REG el;
    int removed;
};

typedef struct _arr_el ARR_EL;

typedef struct _dyn_array DYN_ARRAY;

DYN_ARRAY* initialize_dynarr();

void clear_dynarr(DYN_ARRAY **array);

int64_t get_len_dynarr(DYN_ARRAY **array);

ARR_EL *get_raw_dyarr(DYN_ARRAY **array);

INDEX_REG get_dynarr(int64_t i, DYN_ARRAY **array);

int64_t find_pos_dynarr(int32_t index, DYN_ARRAY **array);

int insert_last_dynarr(INDEX_REG reg, DYN_ARRAY **array);

int insert_ord_dynarr(INDEX_REG reg, DYN_ARRAY **array);

int remove_dynarr(int32_t index, DYN_ARRAY **array);

#endif