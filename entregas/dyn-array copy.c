#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dyn-array copy.h"

/**
 * @brief Estrutura de um array dinâmico
 */
struct _dyn_array {
    int64_t last; /**< @brief Índice do último elemento (válido) da heap*/
    int64_t max_size; /**< @brief Tamanho máximo de arr, aumenta em potências de 2*/
    ARR_EL *arr; /**< @brief Representação da heap como vetor*/
};

DYN_ARRAY* initialize_dynarr(){
    DYN_ARRAY *array = malloc(sizeof(DYN_ARRAY));

    array->last = -1;
    array->max_size = 1;
    array->arr = malloc(sizeof(ARR_EL));
    return array;
}

void clear_dynarr(DYN_ARRAY **array){
    free((*array)->arr);
    free((*array));
    (*array) = NULL;
}

int64_t get_len_dynarr(DYN_ARRAY **array){
    return (*array)->last+1;
}

ARR_EL *get_raw_dyarr(DYN_ARRAY **array){
    return (*array)->arr;
}

INDEX_REG get_dynarr(int64_t i, DYN_ARRAY **array){
    if(i > (*array)->last){
        return (INDEX_REG){0,-1};
    }

    return (*array)->arr[i].removed ? (INDEX_REG){0,-1} : (*array)->arr[i].el;
}

int64_t find_pos_dynarr(int32_t index, DYN_ARRAY **array){
    int64_t beg = 0;
    int64_t end = (*array)->last;

    while(beg <= end){
        int64_t mid = beg + (end-beg)/2;

        if((*array)->arr[mid].el.index == index){
            return (*array)->arr[mid].removed ? -1 : mid;
        }
        else if(index > (*array)->arr[mid].el.index){
            beg = mid+1;
        }
        else{
            end = mid-1;
        }
    }

    return -1;
}

int insert_last_dynarr(INDEX_REG reg, DYN_ARRAY **array){
    if((*array)->last >= (*array)->max_size-1){
        (*array)->max_size = ((*array)->max_size*2);
        (*array)->arr = realloc((*array)->arr, sizeof(ARR_EL) * (*array)->max_size);
    }

    (*array)->arr[++(*array)->last] = (ARR_EL){
        .el = reg,
        .removed = 0
    };

    return 0;
}

int insert_ord_dynarr(INDEX_REG reg, DYN_ARRAY **array){
    if((*array)->last >= (*array)->max_size-1){
        (*array)->max_size = ((*array)->max_size*2);
        (*array)->arr = realloc((*array)->arr, sizeof(ARR_EL) * (*array)->max_size);
    }

    (*array)->arr[++(*array)->last] = (ARR_EL){
        .el = reg,
        .removed = 0
    };

    int64_t i = (*array)->last;

    ARR_EL el_aux = (*array)->arr[i];

    for(; i > 0 && (*array)->arr[i-1].el.index > el_aux.el.index; i--){
        (*array)->arr[i] = (*array)->arr[i-1];
    }

    (*array)->arr[i] = el_aux;

    return 0;
}

int remove_dynarr(int32_t index, DYN_ARRAY **array){
    int64_t i = find_pos_dynarr(index, array);
    
    if(i < 0){
        return -1;
    }

    (*array)->arr[i].removed = 1;
    return 0;
}