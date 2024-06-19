/**
 * @file index-btree-cache.h
 * @brief Header file com as funcionalidades da cache da árvore-b
 *
 * @details A implementação da cache é realizada com uma lista ligada, ao invés de uma lista estática
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 *
 * @version 3.0
 * 
 */

#ifndef INDEX_BTREE_CACHE_H
#define INDEX_BTREE_CACHE_H

#include <stdint.h>

#include "index-btree.h"
#include "index-btree-internals.h"

/**
 * @brief Inicializa a cache da árvore-b
 * 
 * @return [CACHE*] Retorna o ponteiro da cache alocada
 */
CACHE* initialize_cache();

/**
 * @brief Limpa a memória alocada pela cache da árvore-b
 * 
 * @param cache Referência ao ponteiro da cache
 */
void clear_cache(CACHE **cache);

/**
 * @brief Coloca uma página na cache, sem realizar a procura na cache
 * 
 * @param rrn RRN de onde a página se localizaria
 * @param page Referência à página a ser colocada na cache
 * @param btree Referência ao ponteiro da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 *
 * @return [BTREE_PAGE*] Retorna o ponteiro da página dentro da cache, assim se a página for alterada, as mudanças serão
 * refletidas na cache
 */
BTREE_PAGE* put_page_cache(const int32_t rrn, const BTREE_PAGE *page, BTREE **btree, FILE *btree_fptr);

/**
 * @brief Recupera uma página da árvore-B; a função primeiramente verifica se a página está na cache, se ela não estiver na cache
 * A página é recuperada no arquivo e colocada na cache (pressupondo que a página exista no arquivo)
 * 
 * @param rrn RRN da página a ser recuperada
 * @param cache Referência para o ponteiro da cache da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 * 
 * @return [BTREE_PAGE*] Retorna o ponteiro da página dentro da cache, assim se a página for alterada, as mudanças serão
 * refletidas na cache
 */
BTREE_PAGE* get_page(const int32_t rrn, BTREE **btree, FILE *btree_fptr);

#endif