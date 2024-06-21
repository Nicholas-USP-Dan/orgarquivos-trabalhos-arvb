/**
 * @file index-btree-internals.h
 * @brief Header que contém as definições internas da árvore-b; Qualquer arquivo com utilizando esse header tem acesso à todas
 * as definições das estruturas de dados, junto com declarações de tipos internos (como as estruturas de cache)
 *
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 *
 * @version 3.0
 *  
 */

#ifndef INDEX_BTREE_INTERNALS_H
#define INDEX_BTREE_INTERNALS_H

#include <stdint.h>

#include "index-types.h"

#define BTREE_ORDER 4
#define BTREE_MIN_ORDER 1
#define BTREE_PAGE_SIZE 60
#define CACHE_LEN 20
#define BTREE_HEADER_SIZE 13

/**
 * @brief Estrutura representando uma página de uma árvore B
 * 
 * @note Como estruturas em C possuem padding para melhorar performance e evitar erros, elas não terão o tamanho
 * perfeito de 60 bytes, mas esta diferença é pequena e facilita a manipulação das páginas no programa
 * 
 * @note O rrn não está presente na estrutura da página, visto que ele é utilizado como referência à uma página
 */
struct _btree_page {
    int32_t alt;
    INDEX_REG chaves[BTREE_ORDER-1];
    int32_t rrn_filhos[BTREE_ORDER];
    int32_t nro_chaves;
};

/**
 * @brief Estrutura para um nó (elemento) da cache de uma árvore-b virtual
 * 
 */
struct _cache_node {
    struct _btree_page value;
    struct _cache_node *prev, *next;
    int32_t rrn;
};

/**
 * @brief Tipo para um nó (elemento) da cache de uma árvore-b virtual
 * 
 */
typedef struct _cache_node CACHE_NODE;

/**
 * @brief Estrutura para uma cache LRU (Least Recently Used) de pá
 * 
 */
struct _cache {
    CACHE_NODE *head, *tail;
    int count;
};

/**
 * @brief Tipo para a cache da árvore-B
 * 
 */
typedef struct _cache CACHE;

/**
 * @brief Estrutura de uma b-tree virtual (que tem cache, e toda a sua manipulação é feita em memória primária)
 * 
 */
struct _btree {
    CACHE *cache;
    int32_t rrn_root;
    int32_t rrn_prox;
    int32_t key_count;
};

#endif