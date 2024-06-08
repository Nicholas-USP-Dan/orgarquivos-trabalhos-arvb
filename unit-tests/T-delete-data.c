#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "../src/data-file.h"
#include "../src/adts/dyn-array.h"
#include "../src/index-file.h"
#include "../src/utils/removed-list.h"
#include "../src/utils/cabecalho-utils.h"

int main(){
    FILE *data_fptr = fopen("unit-tests/files/binario-test copy.bin", "rb+");

    if(!data_fptr || !check_status(data_fptr)){
        printf("Erro no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);
    DYN_ARRAY *index_arr = generate_index(data_fptr);

    printf("========== Generated index list ==========\n");
    for(int i = 0; i < get_len_dynarr(&index_arr); i++){
        INDEX_REG* ptr = (INDEX_REG*)get_dynarr(i, &index_arr);
        if(ptr) {
            printf("index: %" PRId32 "; offset: 0x%" PRIX64 "\n", ptr->index, ptr->offset);
        }
        else {
            printf("Nulo\n");
        }
    }

    printf("\n");
    printf("========== Loaded removed list ==========\n");
    for(int i = 0; i < get_len_dynarr(&(*rem_list).arr); i++){
        REM_EL* ptr = (REM_EL*)get_dynarr(i, &(*rem_list).arr);
        if(ptr) {
            printf("tam: %" PRId32 "; offset: 0x%" PRIX64 "\n", ptr->tam, ptr->offset);
        }
        else {
            printf("Nulo\n");
        }
    }


    JOGADOR where = (JOGADOR){
        .id = 192985,
        .idade = -1,
        .nome = "",
        .nac = "",
        .clube = ""
    };

    int rem_count = 0;
    delete_data(data_fptr, where, &rem_count, &rem_list, &index_arr);

    printf("\n");
    printf("========== Removed list after operation ==========\n");
    for(int i = 0; i < get_len_dynarr(&(*rem_list).arr); i++){
        REM_EL* ptr = (REM_EL*)get_dynarr(i, &(*rem_list).arr);
        if(ptr) {
            printf("tam: %" PRId32 "; offset: 0x%" PRIX64 "\n", ptr->tam, ptr->offset);
        }
        else {
            printf("Nulo\n");
        }
    }

    write_rem_list(&rem_list, data_fptr);

    clear_dynarr(&index_arr);
    clear_rem_list(&rem_list);

    fclose(data_fptr);
}