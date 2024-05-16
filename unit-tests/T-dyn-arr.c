#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/adts/dyn-sorted-array.h"

int main(){
    DYN_SORTED_ARRAY *array = initialize_dyn_array();

    insert_dyn_array((INDEX_REG){.index = 869, .offset = 12345}, &array);
    insert_dyn_array((INDEX_REG){.index = 960, .offset = 3}, &array);
    insert_dyn_array((INDEX_REG){.index = 750, .offset = 12312345}, &array);
    insert_dyn_array((INDEX_REG){.index = 5674, .offset = 12312345}, &array);
    insert_dyn_array((INDEX_REG){.index = 532, .offset = 12312345}, &array);
    insert_dyn_array((INDEX_REG){.index = 850, .offset = 12312345}, &array);
    insert_dyn_array((INDEX_REG){.index = 751, .offset = 12312345}, &array);
    insert_dyn_array((INDEX_REG){.index = 1000, .offset = 12312345}, &array);
    assert(get_dyn_array(0, &array).index == 532);
    
    remove_dyn_array(960, &array);
    printf("find:960 => i: %" PRId64 "\n", find_pos_dyn_array(960, &array));

    for(int i = 0; i < 8; i++){
        printf("%" PRId32 "\n", get_dyn_array(i, &array).index);
    }

    clear_dyn_array(&array);
    assert(array == NULL);
}