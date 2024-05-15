/**
 * @file min-heap.h
 * @brief Header file para a ADT de fila de prioridade mínima (min-heap) de registro de índices
 * 
 * Este arquivo contém as declarações das funcionalidades comuns de uma lista de prioridades;
 * Esta estrutura guarda os registros de índice utilizados no arquivo de índices do projeto
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 */

#ifndef MINHEAP_H
#define MINHEAP_H

#include "../utils/data-file-utils.h"

/**
 * @brief Tipo da estrutura de min-heap com definição opaca
 */
typedef struct _min_heap MIN_HEAP;

/**
 * @brief Inicializa um ponteiro de min-heap.
 * 
 * @return [MIN_HEAP*] Retorna o ponteiro da heap alocada em memória. 
 */
MIN_HEAP* initialize_minheap();

/**
 * @brief Limpa a memória utilizada por uma min-heap.
 * 
 * @details Essa função limpa a memória alocada para o array da heap e do próprio ponteiro, 
 * além disso atribui o ponteiro como nulo
 * 
 * @param heap Referência para o ponteiro da heap
 */
void clear_minheap(MIN_HEAP **heap);

/**
 * @brief Insere um novo elemento em uma heap.
 * 
 * @param index_reg Elemento a ser adicionado na heap
 * @param heap Referência para o ponteiro da heap
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 O A operação foi realizada com sucesso.
 */
int insert_minheap(INDEX_REG index_reg, MIN_HEAP **heap);

/**
 * @brief Remove o primeiro elemento da heap (menor elemento.)
 * 
 * @param heap Referência para o ponteiro da heap
 * @return [INDEX_REG] Retorna o elemento removido (primeiro elemento) 
 */
INDEX_REG pop_minheap(MIN_HEAP **heap);

/**
 * @brief Verifica se uma heap está vazia.
 * 
 * @param heap Referência para o ponteiro da heap
 * 
 * @retval 1 A heap está vazia
 * @retval 0 A heap não está vazia
 */
int empty_heap(MIN_HEAP **heap);

#endif