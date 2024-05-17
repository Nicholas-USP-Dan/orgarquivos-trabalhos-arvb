#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "index-file.h"

#include "utils/data-file-utils.h"
#include "utils/funcoes_fornecidas.h"
#include "utils/cabecalho-utils.h"
#include "utils/campo-utils.h"
#include "adts/dyn-array.h"

static int comparator(const void* a, const void* b){
    return ((ARR_EL*)a)->el.index - ((ARR_EL*)b)->el.index;
}

int create_index(const char *bin_filename, const char *index_filename){
    // Abra o arquivo no modo leitura binaria
    FILE *data_bfile_fptr = fopen(bin_filename, "rb");
    if(data_bfile_fptr == NULL){
        return -1;
    }

    // Check de status valido
    if(!check_status(data_bfile_fptr)){
        return -1;
    }

    FILE *index_file_fptr = fopen(index_filename, "wb");
    if(index_file_fptr == NULL){
        return -1;
    }

    // Escrever o status do arquivo de índice como instável
    set_campoc('0', index_file_fptr);

    int32_t reg_count = 0;

    // Ler quantidade de registros existentes
    fseek(data_bfile_fptr, NRO_REGARQ_OFFSET, SEEK_SET);
    int32_t nro_reg = get_campo32(data_bfile_fptr);

    // Pular o cabecalho
    fseek(data_bfile_fptr, HEADER_END_OFFSET, SEEK_SET);

    DYN_ARRAY *dyarr = initialize_dynarr();

    // Lê registros até ler todos os registros válidos
    while(reg_count < nro_reg){
        unsigned char rem = get_campoc(data_bfile_fptr);

        if(feof(data_bfile_fptr)){
            break;
        }

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            int32_t reg_size = get_campo32(data_bfile_fptr);
            fseek(data_bfile_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        int64_t offset = ftell(data_bfile_fptr)-1;

        // Pular tamanhoRegistro e Prox
        fseek(data_bfile_fptr, 12, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_bfile_fptr);

        insert_last_dynarr((INDEX_REG){j.id, offset}, &dyarr);

        free_jogador(&j);

        reg_count++;
    }

    qsort(get_raw_dyarr(&dyarr), get_len_dynarr(&dyarr), sizeof(ARR_EL), &comparator);

    for(int i = 0; i < get_len_dynarr(&dyarr); i++){
        INDEX_REG reg = get_dynarr(i, &dyarr);
        set_campo32(reg.index, index_file_fptr);
        set_campo64(reg.offset, index_file_fptr);
    }
    // while(!empty_heap(&heap)){
    //     INDEX_REG reg = pop_minheap(&heap);
    //     set_campo32(reg.index, index_file_fptr);
    //     set_campo64(reg.offset, index_file_fptr);
    // }

    clear_dynarr(&dyarr);

    // Escrever o status do arquivo de índice como estável
    fseek(index_file_fptr, 0, SEEK_SET);
    set_campoc('1', index_file_fptr);
    
    fclose(data_bfile_fptr);
    fclose(index_file_fptr);
    return 0;
}