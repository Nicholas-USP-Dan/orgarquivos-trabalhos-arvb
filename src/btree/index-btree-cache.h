/**
 * @file index-btree-cache.h
 * @brief Header file com todas as funcionalidades que envolvam a cache e manipulação com memória secundária
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
 * @brief Lê uma página no disco e coloca os valores lidos em uma estrutura BTREE_PAGE
 * 
 * @param [out] page Referência da variável que receberá os dados lidos na página
 * @param rrn RRN da página a ser lida
 * @param btree_fptr Ponteiro do arquivo da árvore-b
 *
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int read_disk_page(BTREE_PAGE *const page, const int32_t rrn, FILE *btree_fptr);

/**
 * @brief Escreve uma página da árvore-b (representada como a estrutura BTREE_PAGE) no arquivo da árvore-b
 * 
 * @param rrn RRN da página a ser escrita
 * @param page Referência à página a ser escrita
 * @param btree_fptr Ponteiro para o ponteiro do arquivo da árvore-b
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int update_disk_page(const int32_t rrn, const BTREE_PAGE *page, FILE *btree_fptr);

/**
 * @brief Coloca uma página na cache, sem realizar a procura na cache
 * 
 * @param rrn RRN de onde a página se localizaria
 * @param page Referência à página a ser colocada na cache
 * @param btree Referência ao ponteiro da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 *
 * @return [BTREE_PAGE*] Retorna o ponteiro da página inserida dentro da cache, assim se a página for alterada, as mudanças serão
 * refletidas na cache
 */
BTREE_PAGE* put_page_cache(const int32_t rrn, const BTREE_PAGE *page, BTREE **btree, FILE *btree_fptr);

/**
 * @brief Variação da função put_page_cache, porém sem a escrita de volta na página em passagem do limite da cache
 *
 * @details Seria possível atingir o mesmo propósito desta função por meio de uma flag em put_page_cache que decide se irá escrever ou não, 
 * porém ao escrever uma função dedicada readonly, tenho uma garantia mais forte de não modificar o arquivo.
 * 
 * @note Readonly garante que não haverá escritas no arquivo, assim é possível abrir o arquivo da árvore-b em modo de leitura
 * 
 * @param rrn RRN de onde a página se localizaria
 * @param page Referência à página a ser colocada na cache
 * @param btree Referência ao ponteiro da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 *
 * @return [BTREE_PAGE*] Retorna o ponteiro da página inserida dentro da cache, assim se a página for alterada, as mudanças serão
 * refletidas na cache
 */
BTREE_PAGE* put_page_cache_readonly(const int32_t rrn, const BTREE_PAGE *page, BTREE **btree, FILE *btree_fptr);

/**
 * @brief Cria uma nova página (que não existe no arquivo) na cache
 *
 * @note Esta função é basicamente uma macro que chama put_page_cache com uma página vazia (ao invés de usar get_page)
 *
 * @param rrn RRN de onde a página se localizaria
 * @param btree Referência ao ponteiro da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 *
 * @return [BTREE_PAGE*] Retorna o ponteiro da nova página criada na cache
 */
BTREE_PAGE* create_newpage_cache(const int32_t rrn, BTREE **btree, FILE *btree_fptr);

/**
 * @brief Recupera uma página da árvore-B
 * 
 * @details A função primeiramente verifica se a página está na cache, se ela não estiver na cache
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

/**
 * @brief Versão da função get_page, porém sem a escrita de volta na página em passagem do limite da cache
 * 
 * @note Readonly garante que não haverá escritas no arquivo, assim é possível abrir o arquivo da árvore-b em modo de leitura
 *
 * @param rrn RRN da página a ser recuperada
 * @param cache Referência para o ponteiro da cache da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 * 
 * @return [BTREE_PAGE*] Retorna o ponteiro da página dentro da cache, assim se a página for alterada, as mudanças serão
 * refletidas na cache
 */
BTREE_PAGE* get_page_readonly(const int32_t rrn, BTREE **btree, FILE *btree_fptr);

#endif