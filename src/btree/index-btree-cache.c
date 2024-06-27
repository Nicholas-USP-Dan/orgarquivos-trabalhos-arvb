/**
 * @file index-btree-cache.c
 * @brief Source file com todas as funcionalidades que envolvam a cache e manipulação com memória secundária
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

#include "index-btree-cache.h"

int read_disk_page(BTREE_PAGE *const page, const int32_t rrn, FILE *btree_fptr) {
    // Posiciona o ponteiro do arquivo no rrn correto
    fseek(btree_fptr, (rrn+1) * BTREE_PAGE_SIZE, SEEK_SET);

    // Leitura da página inteira em disco
    unsigned char buffer[BTREE_PAGE_SIZE];
    if(fread(buffer, sizeof(unsigned char), BTREE_PAGE_SIZE, btree_fptr) != BTREE_PAGE_SIZE){
        // Houve algum erro durante a leitura do bloco
        return -1;
    }

    // Transcrição do buffer para uma estrutura de página

    int off = 0; // Offset para a leitura do buffer

    memcpy(&page->alt, &buffer[off], 4); off += 4;
    memcpy(&page->nro_chaves, &buffer[off], 4); off += 4;

    // Leitura dos elementos da página do buffer
    for (int i = 0; i < BTREE_ORDER-1; i++) {
        memcpy(&page->chaves[i].index, &buffer[off], 4); off += 4;
        memcpy(&page->chaves[i].offset, &buffer[off], 8); off += 8;
    }

    memcpy(page->rrn_filhos, &buffer[off], sizeof(int32_t) * BTREE_ORDER);
    off += sizeof(int32_t) * BTREE_ORDER;

    return 0;
}

int update_disk_page(const int32_t rrn, const BTREE_PAGE *page, FILE *btree_fptr){
    // Posiciona o ponteiro do arquivo no rrn correto
    fseek(btree_fptr, (rrn+1) * BTREE_PAGE_SIZE, SEEK_SET);
    
    // Bloco a ser escrito em disco
    unsigned char buffer[BTREE_PAGE_SIZE];

    // Escrita da estrutura da página para o buffer

    int off = 0; // Offset para a escrita no buffer
    memcpy(&buffer[off], &page->alt, 4); off += 4;
    memcpy(&buffer[off], &page->nro_chaves, 4); off += 4;

    // Escrita dos elementos da página no buffer
    for(int i = 0; i < BTREE_ORDER-1; i++){
        memcpy(&buffer[off], &page->chaves[i].index, 4); off += 4;
        memcpy(&buffer[off], &page->chaves[i].offset, 8); off += 8;
    }

    memcpy(&buffer[off], page->rrn_filhos,  sizeof(int32_t) * BTREE_ORDER);
    off += sizeof(int32_t) * BTREE_ORDER;

    // Escrita do buffer no arquivo
    if(fwrite(buffer, sizeof(unsigned char), BTREE_PAGE_SIZE, btree_fptr) != BTREE_PAGE_SIZE){
        // Houve algum erro durante a escrita do bloco
        perror("Erro de escrita: ");
        return -1;
    }

    return 0;
}

CACHE* initialize_cache(){
    CACHE *new_cache = (CACHE*)malloc(sizeof(CACHE));

    new_cache->count = 0;

    // Alocação dos nós sentinelas (2 sentinelas, um no começo e outro no fim)
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
        // Pega a última página da cache e escreve ela no disco

        CACHE_NODE *aux_node = (*btree)->cache->tail->prev;
        if(update_disk_page(aux_node->rrn, &aux_node->value, btree_fptr) == -1){
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

BTREE_PAGE* put_page_cache_readonly(const int32_t rrn, const BTREE_PAGE *page, BTREE **btree, FILE *btree_fptr){
    
    // Cache atingiu tamanho máximo => remover último elemento
    if((*btree)->cache->count >= CACHE_LEN){
        // Pega último elemento da cache e escreve seu valor no disco

        CACHE_NODE *aux_node = (*btree)->cache->tail->prev;

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

BTREE_PAGE* create_newpage_cache(const int32_t rrn, BTREE **btree, FILE *btree_fptr){
    BTREE_PAGE newpage;
    newpage.alt = 0;
    newpage.nro_chaves = 0;
    newpage.rrn_filhos[0] = -1;
    for(int i = 0; i < BTREE_ORDER-1; i++){
        newpage.chaves[i] = (INDEX_REG){-1,-1};
        newpage.rrn_filhos[i+1] = -1;
    }

    return put_page_cache(rrn, &newpage, btree, btree_fptr);
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
        return NULL;
    }

    // Inserir a página na cache
    return put_page_cache(rrn, &page, btree, btree_fptr);
}

BTREE_PAGE* get_page_readonly(const int32_t rrn, BTREE **btree, FILE *btree_fptr){

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
        return NULL;
    }

    return put_page_cache_readonly(rrn, &page, btree, btree_fptr);
}