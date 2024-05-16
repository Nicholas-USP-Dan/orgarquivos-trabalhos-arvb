/**
 * @file min-heap.c
 * @brief Source file da ADT de fila de prioridade mínima (min-heap) de registros de índice
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "min-heap.h"

#define PARENT_INDEX(i) (i-1)/2 /**< @brief Índice do elemento pai do elemento em i*/
#define LCHILD_INDEX(i) i*2+1 /**< @brief Índice do filho à esquerda do elemento em i*/
#define RCHILD_INDEX(i) i*2+2 /**< @brief Índice do filho à direita do elemento em i*/

/**
 * @brief Estrutura de uma min-heap
 */
struct _min_heap {
    int32_t last; /**< @brief Índice do último elemento (válido) da heap*/
    int64_t max_size; /**< @brief Tamanho máximo de arr, aumenta em potências de 2*/
    INDEX_REG *arr; /**< @brief Representação da heap como vetor*/
};

/**
 * @brief Movimento de bubble-up da min-heap.
 * 
 * @details A função bubble_up leva elementos menores do que seus parentes para cima.
 * 
 * @param heap Referência do ponteiro em uma heap
 * @param root Índice do elemento a ser movimentado
 */
static void bubble_up(MIN_HEAP **heap, int root){
    if(root <= 0){
        return;
    }

    INDEX_REG parent = (*heap)->arr[PARENT_INDEX(root)];

    if((*heap)->arr[root].index < parent.index){
        (*heap)->arr[PARENT_INDEX(root)] = (*heap)->arr[root];
        (*heap)->arr[root] = parent;
        bubble_up(heap, PARENT_INDEX(root));
    }
}

/**
 * @brief Movimento de bubble-down da min-heap.
 * 
 * @details A função bubble_down leva elementos maiores do que seus filhos para baixo.
 * 
 * @param heap Referência do ponteiro em uma heap
 * @param root Índice do elemento a ser movimentado
 */
static void bubble_down(MIN_HEAP **heap, int root){
    if(root*2 > (*heap)->last){
        return;
    }

    INDEX_REG maior_reg = (*heap)->arr[root];
    int i_menor = root;
    
    if(LCHILD_INDEX(root) <= (*heap)->last &&
    maior_reg.index > (*heap)->arr[LCHILD_INDEX(root)].index){
        maior_reg = (*heap)->arr[LCHILD_INDEX(root)];
        i_menor = LCHILD_INDEX(root);
    }

    if((RCHILD_INDEX(root)) <= (*heap)->last && 
    maior_reg.index > (*heap)->arr[RCHILD_INDEX(root)].index){
        
        maior_reg = (*heap)->arr[RCHILD_INDEX(root)];
        i_menor = RCHILD_INDEX(root);
    }

    if(i_menor != root){
        (*heap)->arr[i_menor] = (*heap)->arr[root];
        (*heap)->arr[root] = maior_reg;

        bubble_down(heap, i_menor);
    }
    return;
}

MIN_HEAP* initialize_minheap(){
    MIN_HEAP *heap = malloc(sizeof(MIN_HEAP));
    heap->last = -1;
    heap->max_size = 1;
    heap->arr = malloc(sizeof(INDEX_REG));

    return heap;
}

void clear_minheap(MIN_HEAP **heap){
    (*heap)->last = -1;
    (*heap)->max_size = 0;
    free((*heap)->arr);
    free((*heap));
    (*heap) = NULL;
}

int insert_minheap(INDEX_REG index_reg, MIN_HEAP **heap){
    if((*heap)->last >= (*heap)->max_size-1){
        (*heap)->max_size = ((*heap)->max_size*2+1);
        (*heap)->arr = realloc((*heap)->arr, sizeof(INDEX_REG) * (*heap)->max_size);
    }

    (*heap)->arr[++(*heap)->last] = index_reg;

    bubble_up(heap, (*heap)->last);

    return 0;
}

INDEX_REG pop_minheap(MIN_HEAP **heap){
    if(empty_heap(heap)){
        return (INDEX_REG){0,-1};
    }

    INDEX_REG index_aux = (*heap)->arr[0];
    (*heap)->arr[0] = (*heap)->arr[(*heap)->last--];
    bubble_down(heap, 0);
    return index_aux;
}

int empty_heap(MIN_HEAP **heap){
    return (*heap)->last < 0;
}
