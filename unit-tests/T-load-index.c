#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/index-file.h"
#include "../src/utils/cabecalho-utils.h"

int main(){
    DYN_ARRAY* index_arr;

    FILE *index_fptr;
    if(!(index_fptr = fopen("unit-tests/files/index-test.bin", "rb")) || !check_status(index_fptr)){
        return EXIT_FAILURE;
    }

    index_arr = load_index(index_fptr);
    
    fclose(index_fptr);

    assert(index_arr != NULL);

    int64_t len = get_len_dynarr(&index_arr);
    for(int64_t i = 0; i < len; i++){
        INDEX_REG reg = get_dynarr(i, &index_arr);
        printf("index: %" PRId32 "; offset: 0x%" PRIX64 "\n", reg.index, reg.offset);
    }

    clear_dynarr(&index_arr);
}