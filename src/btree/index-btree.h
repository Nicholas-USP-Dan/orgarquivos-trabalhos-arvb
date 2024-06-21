#ifndef INDEX_BTREE_H
#define INDEX_BTREE_H

#include <stdint.h>
#include <stdio.h>

#include "index-types.h"

typedef struct _btree_page BTREE_PAGE;

typedef struct _btree BTREE;

BTREE* initialize_btree();

void clear_btree(BTREE **btree);

int set_btree_cabecalho(const char status, BTREE **btree, FILE *btree_fptr);

int read_btree_cabecalho(BTREE **btree, FILE *btree_fptr);

// BTREE_PAGE* get_page(int32_t rrn, BTREE **btree, FILE *btree_fptr);

int dump_btree_cache(BTREE **btree, FILE *btree_fptr);

/**
 * @brief Procura por um elemento na árvore-b por meio do índice
 *
 * @param index Índice a ser procurado
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @return [int64_t] Retorna o offset do registro no arquivo de dados que contenha o índice procurado
 * @retval -1 O índice não foi achado na página
 */
int64_t search_btree(const int32_t index, BTREE **btree, FILE *btree_fptr);

int insert_btree(INDEX_REG reg, BTREE **btree, FILE *btree_fptr);

#endif