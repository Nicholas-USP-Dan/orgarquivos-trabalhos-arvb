#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "index-file.h"

#include "adts/dyn-array.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"
#include "utils/index-types.h"

static int index_compare(const void* a, const void* b){
    return ((INDEX_REG*)((ARR_EL*)a)->el)->index - ((INDEX_REG*)((ARR_EL*)b)->el)->index;
}

int64_t get_index(const void *index_reg){
    return (int64_t)((INDEX_REG*)index_reg)->index;
}

DYN_ARRAY* generate_index(FILE *data_fptr){
    DYN_ARRAY *index_arr = initialize_dynarr(&get_index);

    int32_t reg_count = 0;

    // Ler quantidade de registros existentes
    fseek(data_fptr, NRO_REGARQ_OFFSET, SEEK_SET);
    int32_t nro_reg = get_campo32(data_fptr);

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
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        int64_t offset = ftell(data_fptr)-1;

        // Pular tamanhoRegistro e Prox
        fseek(data_fptr, 12, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        INDEX_REG *aux_temp = malloc(sizeof(INDEX_REG));
        aux_temp->index = j.id;
        aux_temp->offset = offset;

        insert_back_dynarr(aux_temp, &index_arr);

        free_jogador(&j);

        reg_count++;
    }

    qsort(get_raw_dyarr(&index_arr), get_len_dynarr(&index_arr), sizeof(ARR_EL), &index_compare);

    return index_arr;
}

int write_index(DYN_ARRAY **index_arr, FILE *index_fptr){
    // Escrever o status do arquivo de índice como instável
    set_campoc('0', index_fptr);

    for(int i = 0; i < get_len_dynarr(index_arr); i++){
        INDEX_REG *reg = (INDEX_REG*)get_dynarr(i, index_arr);
        set_campo32(reg->index, index_fptr);
        set_campo64(reg->offset, index_fptr);
    }

    // Escrever o status do arquivo de índice como estável
    fseek(index_fptr, 0, SEEK_SET);
    set_campoc('1', index_fptr);

    return 0;
}

DYN_ARRAY* load_index(FILE *index_fptr){
    DYN_ARRAY *array = initialize_dynarr(&get_index);

    while(1){
        INDEX_REG *reg = malloc(sizeof(INDEX_REG));

        reg->index = get_campo32(index_fptr);
        reg->offset = get_campo64(index_fptr);

        if(feof(index_fptr)){
            break;
        }

        insert_back_dynarr(reg, &array);
    }

    return array;
}