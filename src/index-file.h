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

/**
 * @brief Função para que o dyn_array possa adquirir o índice do registro;
 * É utilizado somente para a criação do array
 * 
 * @param index_reg 
 * @return [int64_t] 
 */
int64_t get_index(const void *index_reg);

/**
 * @brief Função que a partir de um arquivo de dados gera um array de índice
 * 
 * @param data_fptr Ponteiro para o arquivo de dados a ser lido
 * @return [DYN_ARRAY*] Retorna o ponteiro para o array alocado do índice
 */
DYN_ARRAY* generate_index(FILE *data_fptr);

int write_index(DYN_ARRAY **index_arr, FILE *index_fptr);

DYN_ARRAY* load_index(FILE *index_fptr);

// int delete_index(DYN_ARRAY **index_arr, REM_LIST **rem_arr, FILE *data_fptr, FILE *index_fptr, JOGADOR j_search);

#endif