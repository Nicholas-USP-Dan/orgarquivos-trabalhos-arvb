#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/index-file.h"
#include "../src/utils/index-types.h"
#include "../src/utils/cabecalho-utils.h"

int main(){
    DYN_ARRAY* index_arr = initialize_dynarr();

    insert_ord_dynarr((INDEX_REG){6382, 0xe4}, &index_arr);
    insert_ord_dynarr((INDEX_REG){1234, 0x1d}, &index_arr);
    insert_ord_dynarr((INDEX_REG){9876, 0x63}, &index_arr);
    insert_ord_dynarr((INDEX_REG){7319, 0x3a}, &index_arr);
    insert_ord_dynarr((INDEX_REG){3745, 0xae}, &index_arr);
    insert_ord_dynarr((INDEX_REG){10028, 0x1d4}, &index_arr);

    FILE *index_fptr;
    if(!(index_fptr = fopen("unit-tests/temp.bin", "wb"))){
        return EXIT_FAILURE;
    }

    write_index(&index_arr, index_fptr);

    clear_dynarr(&index_arr);
}