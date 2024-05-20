/**
 * @file filter-data.c
 * @brief Implementação da funcionalidade 3 como definido no projeto
 * 
 * O tipo de jogador é utilizado para representar uma busca filtrada (a estrutura contém todos os campos de um registro);
 * Como definido nas especificações do projeto, o id de cada jogador é único, assim, em buscas que filtram o id de um jogador,
 * É possível sair da busca assim que achar-se um registro com o mesmo id que o filtrado
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

int filter_data(FILE *data_fptr, JOGADOR j_query){
    unsigned int mask = get_mask(j_query);

    int reg_count = 0;
    int filter_count = 0;

    // Ler quantidade de registros existentes
    fseek(data_fptr, NRO_REGARQ_OFFSET, SEEK_SET);
    int32_t nro_reg = get_campo32(data_fptr);

    // Pular o cabecalho
    // fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);
    fseek(data_fptr, HEADER_END_OFFSET-NRO_REGARQ_OFFSET, SEEK_CUR);

    // Lê registros até ler todos os registros válidos
    while(reg_count < nro_reg){
        unsigned char rem = get_campoc(data_fptr);

        if(feof(data_fptr)){
            break;
        }

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            int32_t reg_size = get_campo32(data_fptr);
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular tamanhoRegistro e Prox
        fseek(data_fptr, 12, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        // Guarda o resultado da filtragem
        int filter =    (!(mask & IDMASK) || j_query.id == j.id) && 
                        (!(mask & IDADEMASK) || j_query.idade == j.idade) && 
                        (!(mask & NOMEMASK) || strcmp(j_query.nome, j.nome) == 0) && 
                        (!(mask & NACMASK) || strcmp(j_query.nac, j.nac) == 0) && 
                        (!(mask & CLUBEMASK) || strcmp(j_query.clube, j.clube) == 0);

        // Registro passou pelo filtro
        if(filter){
            filter_count++;

            // Chamar function pointer??
            print_jogador(j);
            printf("\n");
        }

        free_jogador(&j);

        // Caso o filtro tenha um campo id, quando o jogador com esse id for encontrado, terminar busca
        if((mask & IDMASK) && j_query.id == j.id){
            break;
        }

        reg_count++;
    }

    // Imprimir: "Registro inexistente." caso nenhum registro passar pelo filtro
    if(filter_count <= 0){
        printf("Registro inexistente.\n\n");
    }

    //printf("\n");
    return 0;
}