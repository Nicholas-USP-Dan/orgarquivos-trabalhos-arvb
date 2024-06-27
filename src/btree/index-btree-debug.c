/**
 * @file index-btree-debug.c
 * @brief Source file de um conjunto de funções para a visualização/extração de uma árvore-b
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 3.0
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "index-btree.h"
#include "index-btree-cache.h"

#include "index-btree-internals.h"

void print_page(BTREE_PAGE *page){
    printf("alt: %" PRId32 "\n", page->alt);

    printf("chaves:\n[\n");
    for(int i = 0; i < BTREE_ORDER-1; i++){
        printf("  index: %" PRId32 "; offset: %" PRId64 "\n", page->chaves[i].index, page->chaves[i].offset);
    }
    printf("]\n");

    printf("filhos:\n[");
    for(int i = 0; i < BTREE_ORDER; i++){
        printf("%" PRId32 "", page->rrn_filhos[i]);
        if(i < BTREE_ORDER-1){
            printf(",");
        }
    }
    printf("]\n");
}

/**
 * @brief Rotina recursiva da função traverse_btree (iteração em pré-ordem na árvore-b)
 * 
 * @param rrn RRN da página a ser lida
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 */
static void traverse_btree_rec(int32_t rrn, BTREE **btree, FILE *btree_fptr){
    if(rrn == -1){
        return;
    }

    // Recupera a página endereçada em rrn

    BTREE_PAGE *page;

    if(!(page = get_page(rrn, btree, btree_fptr))){
        fprintf(stderr, "Erro ao acessar a página %" PRId32 "!\n", rrn);
        return;
    }

    printf("rrn: %" PRId32 "\n", rrn);
    print_page(page);
    printf("\n");

    // Salva o array dos filhos pois a página pode não existir na cache quando ela for chamada
    int32_t child_buffer[BTREE_ORDER];
    memcpy(child_buffer, page->rrn_filhos, sizeof(int32_t) * BTREE_ORDER);

    // Recursão em todos os filhos da página
    for(int i = 0; i < BTREE_ORDER; i++){
        traverse_btree_rec(child_buffer[i], btree, btree_fptr);
    }
}

void traverse_btree(BTREE **btree, FILE *btree_fptr){
    traverse_btree_rec((*btree)->rrn_root, btree, btree_fptr);
}

/**
 * @brief Rotina recursiva da função gen_visual (itera em pré-ordem a árvore-b, gerando um grafo .dot
 * para a visualização da estrutura)
 * 
 * @param rrn RRN para a página a ser lida
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 * @param output_fptr Ponteiro para o arquivo .dot resultante
 */
static void gen_visual_rec(const int32_t rrn, BTREE **btree, FILE *btree_fptr, FILE *output_fptr){
    if(rrn == -1){
        return;
    }

    // Recupera a página endereçada em rrn
    BTREE_PAGE *page;

    if(!(page = get_page_readonly(rrn, btree, btree_fptr))){
        fprintf(stderr, "Erro ao acessar a página %" PRId32 "!\n", rrn);
        return;
    }

    // Declaração da página e seus elementos
    fprintf(output_fptr, "\tpage_%" PRId32 " [label=\"", rrn);
    fprintf(output_fptr, " <C0> ");

    for(int i = 0; i < page->nro_chaves; i++){
        fprintf(output_fptr, "| %" PRId32 " | <C%" PRId32 "> ", page->chaves[i].index, i+1);
    }
    fprintf(output_fptr, "\", ");
    fprintf(output_fptr, "xlabel=\"%" PRId32 "; h=%" PRId32 "\"", rrn, page->alt);
    fprintf(output_fptr, "]\n");


    // Salva o array dos filhos pois a página pode não existir na cache quando ela for chamada
    int32_t child_buffer[BTREE_ORDER];
    memcpy(child_buffer, page->rrn_filhos, sizeof(int32_t) * BTREE_ORDER);

    // Ligações para todos os filhos da página
    for(int i = 0; i < BTREE_ORDER; i++){
        if(child_buffer[i] == -1){
            continue;
        }

        fprintf(output_fptr, "\tpage_%" PRId32 ":C%" PRId32 " -- page_%" PRId32 "\n", rrn, i, child_buffer[i]);
    }

    // Chamada recursiva para os filhos
    for(int i = 0; i < BTREE_ORDER; i++){
        gen_visual_rec(child_buffer[i], btree, btree_fptr, output_fptr);
    }
}

void gen_visual(BTREE **btree, FILE *btree_fptr, FILE *output_fptr){
    // Cabeçalho e estilo do grafo
    fprintf(output_fptr, "graph arv_b {\n");
    fprintf(output_fptr, "\tsplines=false\n");
    fprintf(output_fptr, "\tnode [shape=record]\n\n");

    gen_visual_rec((*btree)->rrn_root, btree, btree_fptr, output_fptr);

    fprintf(output_fptr, "}");
}