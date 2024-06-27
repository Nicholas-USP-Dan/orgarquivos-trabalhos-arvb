/**
 * @file index-btree.h
 * @brief Header file para as funcionalidades envolvendo a árvore-b
 *
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 *
 * @version 3.0
 * 
 */

#ifndef INDEX_BTREE_H
#define INDEX_BTREE_H

#include <stdint.h>
#include <stdio.h>

#include "index-types.h"

typedef struct _btree_page BTREE_PAGE;

typedef struct _btree BTREE;

/**
 * @brief Inicializa a árvore-b
 *
 * @return [BTREE*] Retorna o ponteiro da árvore alocada
 */
BTREE* initialize_btree();

/**
 * @brief Limpa a memória alocada pela árvore-b
 *
 * @param btree Referência ao ponteiro da árvore-b
 */
void clear_btree(BTREE **btree);

/**
 * @brief Atribui valores ao cabeçalho
 *
 * @param status Status do arquivo ('0' => instável; '1' => estável)
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página
 */
int set_btree_cabecalho(const char status, BTREE **btree, FILE *btree_fptr);

/**
 * @brief Lê o cabeçalho de uma árvore-b de um arquivo
 *
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página ou árvore-b está marcado como instável
 */
int read_btree_cabecalho(BTREE **btree, FILE *btree_fptr);

/**
 * @brief Escreve todas as páginas armazenadas na cache no arquivo da árvore-b
 *
 * @details Esta função remove todos os registros da cache, sem limpar a cache em si
 *
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Referência para o arquivo da árvore-b
 *
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página
 */
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

/**
 * @brief Insere um registro em uma árvore-b
 * 
 * @param reg Registro a ser inserido na árvore-b
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 * 
 * @retval 0 A operação foi feita com sucesso
 * @retval -1 Houve um erro durante a operação
 */
int insert_btree(INDEX_REG reg, BTREE **btree, FILE *btree_fptr);

#endif