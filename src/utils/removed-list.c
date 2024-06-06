#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "removed-list.h"

#include "campo-utils.h"
#include "cabecalho-utils.h"

static int64_t get_tam(const void *a){
    return ((REM_EL*)a)->tam;
}

REM_LIST* initialize_rem_list(const enum alloc_met met){
    REM_LIST *list = malloc(sizeof(REM_LIST));
    list->met = met;

    list->arr = initialize_dynarr(&get_tam);

    return list;
}

void clear_rem_list(REM_LIST **list){
    clear_dynarr(&(*list)->arr);
    free(*list);
    (*list) = NULL;
}

REM_LIST* load_rem_list(FILE *data_fptr, const enum alloc_met met){
    REM_LIST *list = initialize_rem_list(met);

    fseek(data_fptr, TOPO_OFFSET, SEEK_SET);
    int64_t offset = get_campo64(data_fptr);

    while(offset != -1){
        fseek(data_fptr, offset+1, SEEK_SET);

        REM_EL *el = malloc(sizeof(REM_EL));
        el->offset = offset;
        el->tam = get_campo32(data_fptr);
        
        insert_back_dynarr(el, &list->arr);

        offset = get_campo64(data_fptr);
    }

    return list;
}

int64_t find_space(const int32_t tam, REM_LIST **list){
    REM_EL el;
    int64_t last_i = get_len_dynarr(&(*list)->arr)-1;
    int64_t out = -1;

    switch ((*list)->met) {
        case FIRST_FIT:
        case BEST_FIT:
            for(int i = 0; i < get_len_dynarr(&(*list)->arr); i++){
                el = (*(REM_EL*)get_dynarr(i, &(*list)->arr));
                if(el.tam >= tam){
                    out = el.offset;
                    break;
                }
            }
            break;
        case WORST_FIT:
            el = (*(REM_EL*)get_dynarr(last_i, &(*list)->arr));
            out = el.tam >= tam ? el.offset : -1;
            break;
        default:
            out = -1;
            break;
    }

    return out;
}

int write_rem_list(FILE *data_fptr, REM_LIST **list){
    fseek(data_fptr, TOPO_OFFSET, SEEK_SET);
    
    int i = 0;
    REM_EL *el = get_dynarr(i++, &(*list)->arr);
    // set_campo64(el->offset, data_fptr);

    while(el){
        set_campo64(el->offset, data_fptr);
        // Seek para o registro e pular para o campo prox
        fseek(data_fptr, el->offset+1+4, SEEK_SET);
        el = get_dynarr(i++, &(*list)->arr);
    }

    set_campo64(-1, data_fptr);
    return 0;
}

static int compare_rem_el(const void* a, const void* b){
    return ((REM_EL*)((ARR_EL*)a)->el)->tam - ((REM_EL*)((ARR_EL*)b)->el)->tam;
}

void sort_rem_list(REM_LIST **list){
    qsort(get_raw_dyarr(&(*list)->arr), get_len_dynarr(&(*list)->arr), sizeof(ARR_EL), &compare_rem_el);
}

int add_rem_list(FILE *data_fptr, REM_LIST **rem_list, int offset){
    fseek(data_fptr, offset, SEEK_SET);
    REM_EL *el = malloc(sizeof(REM_EL));
    el->offset = offset;
    el->tam = get_campo32(data_fptr);

    insert_back_dynarr(el, rem_list);

    sort_rem_list(rem_list);

    return 0;
}