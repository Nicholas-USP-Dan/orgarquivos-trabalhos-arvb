#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "min-heap.h"

#define PARENT_INDEX(i) (i-1)/2
#define LCHILD_INDEX(i) i*2+1
#define RCHILD_INDEX(i) i*2+2

struct _heap {
    int32_t last;
    int32_t max_size;
    INDEX_REG *arr;
};

static void bubble_up(HEAP **heap, int root){
    if(root <= 0){
        return;
    }

    INDEX_REG parent = (*heap)->arr[PARENT_INDEX(root)];

    if((*heap)->arr[root].index < parent.index){
        (*heap)->arr[PARENT_INDEX(root)] = (*heap)->arr[root];
        (*heap)->arr[root] = parent;
        bubble_up(heap, PARENT_INDEX(root));
    }
}

static void bubble_down(HEAP **heap, int root){
    if(root*2 > (*heap)->last){
        return;
    }

    INDEX_REG maior_reg = (*heap)->arr[root];
    int i_menor = root;
    
    if(LCHILD_INDEX(root) <= (*heap)->last &&
    maior_reg.index > (*heap)->arr[LCHILD_INDEX(root)].index){
        maior_reg = (*heap)->arr[LCHILD_INDEX(root)];
        i_menor = LCHILD_INDEX(root);
    }

    if((RCHILD_INDEX(root)) <= (*heap)->last && 
    maior_reg.index > (*heap)->arr[RCHILD_INDEX(root)].index){
        
        maior_reg = (*heap)->arr[RCHILD_INDEX(root)];
        i_menor = RCHILD_INDEX(root);
    }

    if(i_menor != root){
        (*heap)->arr[i_menor] = (*heap)->arr[root];
        (*heap)->arr[root] = maior_reg;

        bubble_down(heap, i_menor);
    }
    return;
}

HEAP* initialize_minheap(){
    HEAP *heap = malloc(sizeof(HEAP));
    heap->last = -1;
    heap->max_size = 1;
    heap->arr = malloc(sizeof(INDEX_REG));

    return heap;
}

void clear_minheap(HEAP **heap){
    (*heap)->last = -1;
    (*heap)->max_size = 0;
    free((*heap)->arr);
    free((*heap));
    (*heap) = NULL;
}

int insert_minheap(INDEX_REG index_reg, HEAP **heap){
    if((*heap)->last >= (*heap)->max_size-1){
        (*heap)->max_size = ((*heap)->max_size*2+1);
        (*heap)->arr = realloc((*heap)->arr, sizeof(INDEX_REG) * (*heap)->max_size);
    }

    (*heap)->arr[++(*heap)->last] = index_reg;

    bubble_up(heap, (*heap)->last);

    return 100;
}

INDEX_REG pop_minheap(HEAP **heap){
    if(empty_heap(heap)){
        return (INDEX_REG){0,-1};
    }

    INDEX_REG index_aux = (*heap)->arr[0];
    (*heap)->arr[0] = (*heap)->arr[(*heap)->last--];
    bubble_down(heap, 0);
    return index_aux;
}

INDEX_REG top_heap(HEAP **heap){
    return empty_heap(heap) ? (INDEX_REG){0,-1} : (*heap)->arr[0];
}

int empty_heap(HEAP **heap){
    return (*heap)->last < 0;
}
