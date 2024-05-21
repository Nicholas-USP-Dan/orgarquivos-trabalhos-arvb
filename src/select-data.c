/**
 * @file select-data-file.c
 * @brief Implementação da funcionalidade 2 como definida no projeto
 * 
 * Nesta funcionalidade, o programa recebe um arquivo binário com dados de jogadores,
 * e imprime na tela o nome, nacionalidade e clube de cada um dos jogadores
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

int select_data(FILE *data_fptr){
    int reg_count = 0;

    // Ler quantidade de registros existentes
    fseek(data_fptr, NRO_REGARQ_OFFSET, SEEK_SET);
    int32_t nro_reg = get_campo32(data_fptr);
    // fread(&nro_reg, 4, 1, data_fptr);

    // Pular o cabecalho
    fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);

    // Lê registros até ler todos os registros válidos
    while(reg_count < nro_reg){
        unsigned char rem = get_campoc(data_fptr);

        if(feof(data_fptr)){
            break;
        }

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            int32_t reg_size = get_campo32(data_fptr);
            // fread(&reg_size, 4, 1, data_fptr);
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular tamanhoRegistro e Prox
        fseek(data_fptr, 12, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        print_jogador(j);
        printf("\n");

        free_jogador(&j);

        reg_count++;
    }

    // Imprimir: "Registro inexistente." caso nenhum registro seja imprimido
    if(nro_reg <= 0){
        printf("Registro inexistente.\n\n");
    }
    
    return 0;
}