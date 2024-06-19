/**
 * @file index-btree-debug.h
 * @brief Header file de um conjunto de funções para a visualização/extração da árvore-b
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 3.0
 * 
 */

#ifndef INDEX_BTREE_DEBUG_H
#define INDEX_BTREE_DEBUG_H

#include <stdio.h>

#include "index-btree.h"

/**
 * @brief Imprime uma página no stdout
 * 
 * @param page Referência para a página a ser removida
 */
void print_page(BTREE_PAGE *page);

/**
 * @brief Iteração pela árvore-b em pré-ordem
 * 
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 */
void traverse_btree(BTREE **btree, FILE *btree_fptr);

/**
 * @brief Gera uma visualização da árvore-b como um arquivo .dot
 * 
 * @details Possuindo o arquivo .dot, é necessário rodar um programa que interprete o .dot e gere
 * a visualização da árvore-b, recomenda-se utilizar o programa dot com o tipo de arquivo de saída .svg
 * 
 * @param btree Referência para a árvore-b a ser visualizada
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 * @param output_fptr Ponteiro para o arquivo .dot resultante
 */
void gen_visual(BTREE **btree, FILE *btree_fptr, FILE *output_fptr);

#endif