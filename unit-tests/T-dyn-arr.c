#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include <stdint.h>
#include "../src/adts/dyn-array.h"

static int64_t simple_index(const void* a){
    return *((int*)a);
}

int main(){
    DYN_ARRAY *array = initialize_dynarr(simple_index);

    int *val;

    val = malloc(sizeof(int)); *val = 960;
    insert_ord_dynarr(val, &array);

    val = malloc(sizeof(int)); *val = 750;
    insert_ord_dynarr(val, &array);

    val = malloc(sizeof(int)); *val = 5674;
    insert_ord_dynarr(val, &array);

    val = malloc(sizeof(int)); *val = 532;
    insert_ord_dynarr(val, &array);

    val = malloc(sizeof(int)); *val = 850;
    insert_ord_dynarr(val, &array);

    val = malloc(sizeof(int)); *val = 751;
    insert_ord_dynarr(val, &array);

    val = malloc(sizeof(int)); *val = 1000;
    insert_ord_dynarr(val, &array);

    printf("%d\n", *((int*)get_dynarr(0, &array)));

    assert(*((int*)get_dynarr(0, &array)) == 532);
    
    remove_dynarr(960, &array);
    printf("find:960 => i: %" PRId64 "\n", find_pos_dynarr(960, &array));
    printf("find:850 => i: %" PRId64 "\n", find_pos_dynarr(1000, &array));

    for(int i = 0; i < 7; i++){
        int* ptr = (int*)get_dynarr(i, &array);
        if(ptr) printf("%" PRId32 "\n", *ptr);
        else printf("Nulo\n");
    }

    clear_dynarr(&array);
    assert(array == NULL);
}