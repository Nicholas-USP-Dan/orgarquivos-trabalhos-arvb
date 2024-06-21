/**
 * @file index-btree-cache.c
 * @brief Source file com as funcionalidades da cache da árvore-b
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 *
 * @version 3.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "index-btree.h"

#include "index-btree-internals.h"
#include "index-btree-utils.h"

#include "index-btree-cache.h"

CACHE* initialize_cache(){
    CACHE *new_cache = (CACHE*)malloc(sizeof(CACHE));

    new_cache->count = 0;
    new_cache->head = (CACHE_NODE*)malloc(sizeof(CACHE_NODE));
    new_cache->tail = (CACHE_NODE*)malloc(sizeof(CACHE_NODE));

    // Atribuição dos ponteiros dos sentinelas
    new_cache->head->next = new_cache->tail;
    new_cache->head->prev = new_cache->tail;
    new_cache->tail->next = new_cache->head;
    new_cache->tail->prev = new_cache->head;

    return new_cache;
}

void clear_cache(CACHE **cache){
    // Limpeza dos nós da cache
    CACHE_NODE* node_aux = (*cache)->head->next;
    while(node_aux != (*cache)->tail){
        CACHE_NODE *node_temp = node_aux;
        node_aux = node_aux->next;
        free(node_temp);
    }

    // Limpeza dos sentinelas e do ponteiro para a cache
    free((*cache)->head);
    free((*cache)->tail);
    free(*cache);
    *cache = NULL;
}

BTREE_PAGE* put_page_cache(const int32_t rrn, const BTREE_PAGE *page, BTREE **btree, FILE *btree_fptr){
    
    // Cache atingiu tamanho máximo => remover último elemento
    if((*btree)->cache->count >= CACHE_LEN){
        // Pega último elemento da cache e escreve seu valor no disco

        // printf("Remove last element in cache\n");
        CACHE_NODE *aux_node = (*btree)->cache->tail->prev;
        if(update_disk_page(aux_node->rrn, &aux_node->value, btree_fptr) == -1){
            printf("Erro ao dar update\n");
            return NULL;
        }

        // Reposiciona os ponteiros e limpa a memória do item (nó) da cache
        (*btree)->cache->tail->prev = aux_node->prev;
        aux_node->prev->next = (*btree)->cache->tail;
        free(aux_node);

        (*btree)->cache->count--;
    }

    // Criação do novo nó na cache e posicionamento na frente da cache
    CACHE_NODE *new_node = (CACHE_NODE*)malloc(sizeof(CACHE_NODE));
    new_node->value = *page;
    new_node->rrn = rrn;

    new_node->prev = (*btree)->cache->head;
    new_node->next = (*btree)->cache->head->next;

    (*btree)->cache->head->next->prev = new_node;

    (*btree)->cache->head->next = new_node;
    (*btree)->cache->count++;

    // Retorna o elemento obtido pela leitura
    return &new_node->value;
}

BTREE_PAGE* get_page(const int32_t rrn, BTREE **btree, FILE *btree_fptr){

    // Procura a página (referenciada pelo rrn) dentro da cache
    CACHE_NODE* node_aux = (*btree)->cache->head->next;
    while(node_aux != (*btree)->cache->tail){
        if(rrn == node_aux->rrn){
            // Achou a página na cache => colocar ele no começo da lista e retornar a página

            node_aux->prev->next = node_aux->next;
            node_aux->next->prev = node_aux->prev;

            node_aux->prev = (*btree)->cache->head;
            node_aux->next = (*btree)->cache->head->next;
            (*btree)->cache->head->next->prev = node_aux;
            (*btree)->cache->head->next = node_aux;
            return &node_aux->value;
        }

        node_aux = node_aux->next;
    }

    // Cache-miss, deve-se adicionar a página na cache

    BTREE_PAGE page;
    if(read_disk_page(&page, rrn, btree_fptr) == -1){
        // Erro na leitura da página da árvore-b
        printf("oioi\n");
        return NULL;
    }

    return put_page_cache(rrn, &page, btree, btree_fptr);
}