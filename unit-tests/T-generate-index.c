#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/index-file.h"

#include "../src/adts/dyn-array.h"

#include "../src/utils/cabecalho-utils.h"

int main(){
    FILE *data_fptr;
    if(!(data_fptr = fopen("unit-tests/files/binario-test.bin", "rb")) || !check_status(data_fptr)){
        perror(NULL);
        return EXIT_FAILURE;
    }

    DYN_ARRAY *index_arr = generate_index(data_fptr);

    if(index_arr == NULL) perror(NULL);

    assert(index_arr != NULL);

    fclose(data_fptr);

    int64_t len = get_len_dynarr(&index_arr);
    for(int64_t i = 0; i < len; i++){
        INDEX_REG reg = get_dynarr(i, &index_arr);
        printf("index: %" PRId32 "; offset: 0x%" PRIX64 "\n", reg.index, reg.offset);
    }

    clear_dynarr(&index_arr);
}