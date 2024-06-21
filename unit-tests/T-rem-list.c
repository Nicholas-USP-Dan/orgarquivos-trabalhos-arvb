#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/utils/removed-list.h"

int main(){
    REM_LIST *rem_list = initialize_rem_list(BEST_FIT);
    REM_EL *el;

    el = malloc(sizeof(REM_EL)); el->tam = 10; el->offset = 1;
    insert_back_dynarr(el, &(*rem_list).arr);

    el = malloc(sizeof(REM_EL)); el->tam = 20; el->offset = 2;
    insert_back_dynarr(el, &(*rem_list).arr);

    el = malloc(sizeof(REM_EL)); el->tam = 40; el->offset = 3;
    insert_back_dynarr(el, &(*rem_list).arr);
    
    el = malloc(sizeof(REM_EL)); el->tam = 30; el->offset = 5;
    insert_back_dynarr(el, &(*rem_list).arr);

    el = malloc(sizeof(REM_EL)); el->tam = 5; el->offset = 6;
    insert_back_dynarr(el, &(*rem_list).arr);

    el = malloc(sizeof(REM_EL)); el->tam = 25; el->offset = 4;
    insert_back_dynarr(el, &(*rem_list).arr);

    sort_rem_list(&rem_list);

    int32_t expected[6] = {5,10,20,25,30,40};
    for(int i = 0; i < 6; i++){
        REM_EL* ptr = (REM_EL*)get_dynarr(i, &(*rem_list).arr);
        if(ptr) {
            printf("tam: %" PRId32 "; offset: %" PRId64 "\n", ptr->tam, ptr->offset);
            assert(expected[i] == ptr->tam);
        }
        else {
            printf("Nulo\n");
            assert(expected[i] == -1);
        }
    }

    REM_EL space;
    space = find_space(21, &rem_list);
    assert(space.offset == 4);

    space = find_space(5, &rem_list);
    assert(space.offset == 6);

    space = find_space(45, &rem_list);
    assert(space.offset == -1);
    
    clear_rem_list(&rem_list);

    assert(rem_list == NULL);
}