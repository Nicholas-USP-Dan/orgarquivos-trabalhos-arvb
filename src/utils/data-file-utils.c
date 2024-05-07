/**
 * @file data-file-utils.c
 * @brief Source file de algumas utilidades gerais do arquivo binário de dados
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

#include "campo-utils.h"
#include "data-file-utils.h"
#include "funcoes_fornecidas.h"

/**
 * @brief Instância de um jogador vazio/nulo, representa uma busca "vazia"
 */
const JOGADOR jNil = {
    .id = -1,
    .idade = -1,
    .nome = "",
    .nac = "",
    .clube = ""
};

/**
 * @brief Array com todos os nomes dde campos válidos para realizar a filtragem
 */
const char *CAMPO_LIST[5] = {
    "id",
    "idade",
    "nomeJogador",
    "nacionalidade",
    "nomeClube"
};

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

JOGADOR read_query(unsigned int *mask){
    JOGADOR j_query = jNil;

    // Quantidade de campos a serem lidos
    int m;
    scanf("%d", &m);

    (*mask) = 0;

    // Ler o conjunto nomeCampo e valorCampo, atribuir o campo no jogador "filtro" junto com o bit da mascara correspondente
    for(int i = 0; i < m; i++){
        char campo[BUFFER_SIZE];
        scanf("%s", campo);

        if(strcmp(campo, CAMPO_LIST[0]) == 0){
            scanf("%" PRId32, &j_query.id);
            (*mask) |= IDMASK;
        }
        else if(strcmp(campo, CAMPO_LIST[1]) == 0){
            scanf("%" PRId32, &j_query.idade);
            (*mask) |= IDADEMASK;
        }
        else if(strcmp(campo, CAMPO_LIST[2]) == 0){
            j_query.nome = malloc(sizeof(char) * BUFFER_SIZE);
            scan_quote_string(j_query.nome);

            (*mask) |= NOMEMASK;
        }
        else if(strcmp(campo, CAMPO_LIST[3]) == 0){
            j_query.nac = malloc(sizeof(char) * BUFFER_SIZE);
            scan_quote_string(j_query.nac);

            (*mask) |= NACMASK;
        }
        else if(strcmp(campo, CAMPO_LIST[4]) == 0){
            j_query.clube = malloc(sizeof(char) * BUFFER_SIZE);
            scan_quote_string(j_query.clube);
            
            (*mask) |= CLUBEMASK;
        }
        else{
            break;
        }
    }

    return j_query;
}