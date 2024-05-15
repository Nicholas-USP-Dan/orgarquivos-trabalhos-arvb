#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

#include "../src/utils/data-file-utils.h"
#include "../src/adts/min-heap.h"

int main(){
    MIN_HEAP *heap = initialize_minheap();

    insert_minheap((INDEX_REG){2,123}, &heap);

    printf("Testando insercao da heap...\n");
    assert(empty_heap(&heap) == 0);

    INDEX_REG reg = pop_minheap(&heap);

    printf("Testando remocao da heap...\n");
    assert(reg.offset == 123);

    assert(empty_heap(&heap) == 1);

    printf("Testando insercoes apos esvaziamento da heap...\n");
    insert_minheap((INDEX_REG){10,123}, &heap);
    insert_minheap((INDEX_REG){5,1234}, &heap);
    insert_minheap((INDEX_REG){4,1234}, &heap);
    insert_minheap((INDEX_REG){6,1234}, &heap);
    insert_minheap((INDEX_REG){20,1234}, &heap);

    reg = pop_minheap(&heap);
    assert(reg.index == 4);

    reg = pop_minheap(&heap);
    assert(reg.index == 5);

    reg = pop_minheap(&heap);
    assert(reg.index == 6);

    printf("Testando ordenamento da heap...\n");
    insert_minheap((INDEX_REG){1,1234}, &heap);

    reg = pop_minheap(&heap);
    assert(reg.index == 1);

    reg = pop_minheap(&heap);
    assert(reg.index == 10);

    reg = pop_minheap(&heap);
    assert(reg.index == 20);

    printf("Testando remocao de heap vazia...\n");
    reg = pop_minheap(&heap);
    assert(reg.index == 0);

    clear_minheap(&heap);

    printf("Testando limpeza de memoria da heap...\n");
    assert(heap == NULL);
}