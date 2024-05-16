#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

// #include "../src/utils/data-file-utils.h"
#include "../src/index-file.h"
// #include "../src/adts/dyn-sorted-array.h"

int main(){
    DYN_SORTED_ARRAY* array;
    array = load_index_file("unit-tests/index-test.bin");

    assert(array != NULL);

    int64_t len = length_dyn_array(&array);
    for(int64_t i = 0; i < len; i++){
        INDEX_REG reg = get_dyn_array(i, &array);
        printf("index: %" PRId32 "; offset: %" PRId64 "\n", reg.index, reg.offset);
    }

    clear_dyn_array(&array);
}