/**
 * @file select-btree-data.c
 * @brief Implementação da funcionalidade 9 - Busca pelos registros em um arquivo de dados por meio de um critério de busca
 * utilizando árvore-b
 *
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 3.0
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-utils.h"

#include "btree-func.h"
#include "btree/index-btree.h"

int select_btree_data(FILE *data_fptr, BTREE **btree, FILE *btree_fptr, const JOGADOR where){
    //caso tenhamos o campo id, procuraremos o registro pelo índice na arvore-b
    if(where.id != jNil.id){
        int64_t offset = search_btree(where.id, btree, btree_fptr);

        if(offset == -1){
            printf("Registro inexistente.\n\n");
        }

        fseek(data_fptr, offset + 13, SEEK_SET);
        JOGADOR j = read_jogador_data(data_fptr);

        print_jogador(j);
        printf("\n");
    }else{
        select_data(data_fptr, where);
    }

    return 0;
}