#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

// #include "../src/utils/data-file-utils.h"
#include "../src/index-file.h"
// #include "../src/adts/dyn-sorted-array.h"

int main(){
    DYN_ARRAY* array;
    array = load_index("unit-tests/index-test.bin");

    assert(array != NULL);

    int64_t len = get_len_dynarr(&array);
    for(int64_t i = 0; i < len; i++){
        INDEX_REG reg = get_dynarr(i, &array);
        printf("index: %" PRId32 "; offset: 0x%" PRIX64 "\n", reg.index, reg.offset);
    }

    clear_dynarr(&array);
}