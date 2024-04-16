/**
 * @file data-file-utils.c
 * @brief Source file de algumas utilidades gerais do arquivo binário de dados
 * 
 * Alguma descrição específica sobre o arquivo...
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "campo-utils.h"
#include "data-file.h"

void free_jogador(JOGADOR *j){
    free(j->nome);
    free(j->nac);
    free(j->clube);
}

void print_jogador(JOGADOR j){
    // printf("ID do Jogador: %" PRId32 "\n", j.id);

    // printf("Idade do Jogador: %" PRId32 "\n", j.idade);

    char *nome = strcmp(j.nome, "") != 0 ? j.nome : "SEM DADO";
    printf("Nome do Jogador: %s\n", nome);

    char *nac = strcmp(j.nac, "") != 0 ? j.nac : "SEM DADO";
    printf("Nacionalidade do Jogador: %s\n", nac);

    char *clube = strcmp(j.clube, "") != 0 ? j.clube : "SEM DADO";
    printf("Clube do Jogador: %s\n", clube);
}

JOGADOR read_jogador_data(FILE *fptr){
    JOGADOR j;
    j.id = get_campo32(fptr);
    j.idade = get_campo32(fptr);
    j.nome = get_campo_str(fptr);
    j.nac = get_campo_str(fptr);
    j.clube = get_campo_str(fptr);

    return j;
}