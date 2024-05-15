#ifndef MINHEAP_H
#define MINHEAP_H

#include "data-file-utils.h"

typedef struct _heap HEAP;

HEAP* initialize_minheap();

void clear_minheap(HEAP **heap);

int insert_minheap(INDEX_REG index_reg, HEAP **heap);

INDEX_REG pop_minheap(HEAP **heap);

int empty_heap(HEAP **heap);

#endif