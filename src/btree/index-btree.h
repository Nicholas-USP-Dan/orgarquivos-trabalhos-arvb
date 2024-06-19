#ifndef INDEX_BTREE_H
#define INDEX_BTREE_H

#include <stdint.h>
#include <stdio.h>

#include "index-types.h"

typedef struct _btree_page BTREE_PAGE;

typedef struct _btree BTREE;

BTREE_PAGE* get_page(int32_t rrn, BTREE **btree, FILE *btree_fptr);

BTREE* initialize_btree();

int set_btree_cabecalho(const char status, BTREE **btree, FILE *btree_fptr);

int read_btree_cabecalho(BTREE **btree, FILE *btree_fptr);

void clear_btree(BTREE **btree);

int dump_btree_cache(BTREE **btree, FILE *btree_fptr);

int insert_btree(INDEX_REG reg, BTREE **btree, FILE *btree_fptr);

#endif