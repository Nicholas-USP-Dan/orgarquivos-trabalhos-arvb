#ifndef INDEX_BTREE_UTILS_H
#define INDEX_BTREE_UTILS_H

#include <stdint.h>

#include "index-btree.h"

/**
 * @brief Lê uma página de uma árvore-b em um arquivo em disco
 * 
 * @param [out] page Referência para variável que receberá os dados lidos da função
 * @param rrn Posição da página no arquivo
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 * 
 * @retval 0 A leitura da página foi feita com sucesso
 * @retval -1 Houve um erro durante a leitura da página 
 */
int read_disk_page(BTREE_PAGE *const page, const int32_t rrn, FILE *btree_fptr);

/**
 * @brief Escreve uma página de uma árvore-b em disco
 * 
 * @param rrn Posição da página do arquivo a ser escrito
 * @param page Referência à página a ser escrita no arquivo
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 * 
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página
 */
int update_disk_page(const int32_t rrn, const BTREE_PAGE *page, FILE *btree_fptr);

#endif