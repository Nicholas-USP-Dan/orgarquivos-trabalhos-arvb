#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dyn-array.h"

/**
 * @brief Estrutura de um array dinâmico
 */
struct _dyn_sorted_array {
    int64_t last; /**< @brief Índice do último elemento (válido) da heap*/
    int64_t max_size; /**< @brief Tamanho máximo de arr, aumenta em potências de 2*/
    struct arr_el {
        INDEX_REG el;
        int removed;
    } *arr; /**< @brief Representação da heap como vetor*/
};

DYN_SORTED_ARRAY* initialize_dyn_array(){
    DYN_SORTED_ARRAY *array = malloc(sizeof(DYN_SORTED_ARRAY));

    array->last = -1;
    array->max_size = 1;
    array->arr = malloc(sizeof(struct arr_el));
    return array;
}

void clear_dyn_array(DYN_SORTED_ARRAY **array){
    free((*array)->arr);
    free((*array));
    (*array) = NULL;
}

INDEX_REG get_dyn_array(int64_t i, DYN_SORTED_ARRAY **array){
    if(i > (*array)->last){
        return (INDEX_REG){0,-1};
    }

    return (*array)->arr[i].removed ? (INDEX_REG){0,-1} : (*array)->arr[i].el;
}

int64_t find_pos_dyn_array(int32_t index, DYN_SORTED_ARRAY **array){
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

int insert_dyn_array(INDEX_REG reg, DYN_SORTED_ARRAY **array){
    if((*array)->last >= (*array)->max_size-1){
        (*array)->max_size = ((*array)->max_size*2);
        (*array)->arr = realloc((*array)->arr, sizeof(struct arr_el) * (*array)->max_size);
    }

    struct arr_el el = {
        .el = reg,
        .removed = 0
    };
    (*array)->arr[++(*array)->last] = el;

    int64_t i = (*array)->last;
    struct arr_el el_aux = (*array)->arr[i];
    for(; i > 0 && (*array)->arr[i-1].el.index > el_aux.el.index; i--){
        (*array)->arr[i] = (*array)->arr[i-1];
    }
    (*array)->arr[i] = el_aux;

    return 0;
}

int remove_dyn_array(int32_t index, DYN_SORTED_ARRAY **array){
    int64_t i = find_pos_dyn_array(index, array);
    
    if(i < 0){
        return -1;
    }

    (*array)->arr[i].removed = 1;
    return 0;
}

int64_t length_dyn_array(DYN_SORTED_ARRAY **array){
    return (*array)->last+1;
}