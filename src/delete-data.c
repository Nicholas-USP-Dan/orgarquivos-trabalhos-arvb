#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "adts/dyn-array.h"
#include "data-file.h"

#include "index-file.h"
#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"
#include "utils/removed-list.h"

int delete_data(FILE *data_fptr, const JOGADOR where, int *quant_rem, REM_LIST **rem_list, DYN_ARRAY **index_arr){
    // Pular o cabecalho
    fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);

    // O campo id é pesquisado
    if(where.id != jNil.id){
        int64_t id_index;

        // Verificar se o índice está na lista de índices
        if((id_index = find_pos_dynarr(where.id, index_arr)) == -1){
            return 0;
        }

        // Recuperar o registro da lista de índices
        INDEX_REG *reg = (INDEX_REG*)get_dynarr(id_index, index_arr);
        int64_t offset = reg->offset;

        // Ir até o registro do índice e pular o campo removido
        fseek(data_fptr, reg->offset+1, SEEK_SET);

        // Ler o tamanho do registro
        int32_t tam = get_campo32(data_fptr);

        // Pular o campo Prox
        fseek(data_fptr, 8, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        // Registro passou pelo filtro
        if(pass_where(j, where)){
            (*quant_rem)++;
            // Cria e coloca o registro na lista de removidos
            REM_EL *el = (REM_EL*)malloc(sizeof(REM_EL));
            el->offset = offset; el->tam = tam;
            insert_ord_dynarr(el, &(*rem_list)->arr);

            // Remove o elemento na lista de índice
            remove_dynarr(id_index, index_arr);

            // Volta e atribui o campo removido como '1' (registro está removido)
            long curr_offset = ftell(data_fptr);
            fseek(data_fptr, curr_offset-tam, SEEK_SET);
            set_campoc('1', data_fptr);
            fseek(data_fptr, curr_offset, SEEK_CUR);
        }

        free_jogador(&j);

        return 0;
    }

    while(1){
        int64_t offset = ftell(data_fptr);
        unsigned char rem = get_campoc(data_fptr);

        if(feof(data_fptr)){
            break;
        }

        int32_t reg_size = get_campo32(data_fptr);

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular Prox
        fseek(data_fptr, 8, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);
        
        // Registro passou pelo filtro
        if(pass_where(j, where)){
            (*quant_rem)++;
            // Cria e coloca na lista de removidos o elemento
            REM_EL *el = (REM_EL*)malloc(sizeof(REM_EL));
            el->offset = offset; el->tam = reg_size;
            insert_ord_dynarr(el, &(*rem_list)->arr);

            // Remove o elemento na lista de índice
            remove_dynarr(find_pos_dynarr(j.id, index_arr), index_arr);

            // Volta e atribui o campo removido como '1' (registro está removido)
            long curr_offset = ftell(data_fptr);
            fseek(data_fptr, curr_offset-reg_size, SEEK_SET);
            set_campoc('1', data_fptr);

            // Retorna para o final do registro
            fseek(data_fptr, curr_offset, SEEK_SET);
        }

        free_jogador(&j);
    }

    //printf("\n");
    return 0;
}