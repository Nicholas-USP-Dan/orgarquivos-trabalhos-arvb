#ifndef INDEX_UTILS
#define INDEX_UTILS

#include <stdint.h>

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

#endif