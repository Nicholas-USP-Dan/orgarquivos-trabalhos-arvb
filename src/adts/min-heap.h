#ifndef MINHEAP_H
#define MINHEAP_H

#include "../utils/data-file-utils.h"

typedef struct _min_heap MIN_HEAP;

MIN_HEAP* initialize_minheap();

void clear_minheap(MIN_HEAP **heap);

int insert_minheap(INDEX_REG index_reg, MIN_HEAP **heap);

INDEX_REG pop_minheap(MIN_HEAP **heap);

int empty_heap(MIN_HEAP **heap);

#endif