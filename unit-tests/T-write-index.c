#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/index-file.h"
#include "../src/utils/index-types.h"

int main(){
    DYN_ARRAY* index_arr = initialize_dynarr(&get_index);

    INDEX_REG *reg;
    reg = (INDEX_REG*)malloc(sizeof(INDEX_REG)); reg->index=6382; reg->offset=0xe4;
    insert_ord_dynarr(reg, &index_arr);

    reg = (INDEX_REG*)malloc(sizeof(INDEX_REG)); reg->index=1234; reg->offset=0x1d;
    insert_ord_dynarr(reg, &index_arr);

    reg = (INDEX_REG*)malloc(sizeof(INDEX_REG)); reg->index=9876; reg->offset=0x63;
    insert_ord_dynarr(reg, &index_arr);

    reg = (INDEX_REG*)malloc(sizeof(INDEX_REG)); reg->index=7319; reg->offset=0x3a;
    insert_ord_dynarr(reg, &index_arr);

    reg = (INDEX_REG*)malloc(sizeof(INDEX_REG)); reg->index=3745; reg->offset=0xae;
    insert_ord_dynarr(reg, &index_arr);

    reg = (INDEX_REG*)malloc(sizeof(INDEX_REG)); reg->index=10028; reg->offset=0x1d4;
    insert_ord_dynarr(reg, &index_arr);

    FILE *index_fptr;
    if(!(index_fptr = fopen("unit-tests/temp.bin", "wb"))){
        return EXIT_FAILURE;
    }

    write_index(&index_arr, index_fptr);

    clear_dynarr(&index_arr);

    fclose(index_fptr);
}