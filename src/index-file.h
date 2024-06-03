#ifndef INDEX_FILE_H
#define INDEX_FILE_H

#include <stdio.h>

#include "adts/dyn-array.h"

/**
 * @brief Estrutura que contém os dados de um registro de índice
 */
struct _index_reg {
    int32_t index;
    int64_t offset;
};

/**
 * @brief Tipo da estrutura representando os dados de um registro de índice
 * 
 */
typedef struct _index_reg INDEX_REG;

int64_t get_index(const void *index_reg);

DYN_ARRAY* generate_index(FILE *data_fptr);

int write_index(DYN_ARRAY **index_arr, FILE *index_fptr);

DYN_ARRAY* load_index(FILE *index_fptr);

int select_index(FILE *data_fptr, FILE *index_fptr, int32_t id);

#endif