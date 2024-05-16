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
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-file-utils.h"
#include "utils/funcoes_fornecidas.h"
#include "utils/cabecalho-utils.h"
#include "utils/campo-utils.h"

int select_data(const char *bin_filename){
    // Abra o arquivo no modo leitura binaria
    FILE *fptr = fopen(bin_filename, "rb");
    if(fptr == NULL){
        return -1;
    }

    // Check de status valido
    if(!check_status(fptr)){
        return -1;
    }

    int reg_count = 0;

    // Ler quantidade de registros existentes
    fseek(fptr, NRO_REGARQ_OFFSET, SEEK_SET);
    int32_t nro_reg = get_campo32(fptr);
    // fread(&nro_reg, 4, 1, fptr);

    // Pular o cabecalho
    fseek(fptr, HEADER_END_OFFSET, SEEK_SET);

    // Lê registros até ler todos os registros válidos
    while(reg_count < nro_reg){
        unsigned char rem = get_campoc(fptr);

        if(feof(fptr)){
            break;
        }

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            int32_t reg_size = get_campo32(fptr);
            // fread(&reg_size, 4, 1, fptr);
            fseek(fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular tamanhoRegistro e Prox
        fseek(fptr, 12, SEEK_CUR);

        JOGADOR j = read_jogador_data(fptr);

        print_jogador(j);
        printf("\n");

        free_jogador(&j);

        reg_count++;
    }

    // Imprimir: "Registro inexistente." caso nenhum registro seja imprimido
    if(nro_reg <= 0){
        printf("Registro inexistente.\n\n");
    }
    
    fclose(fptr);
    return 0;
}