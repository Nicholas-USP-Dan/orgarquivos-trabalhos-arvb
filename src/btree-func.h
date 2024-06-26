#ifndef BTREE_FUNC_H
#define BTREE_FUNC_H

#include <stdio.h>
#include "btree/index-btree.h"
#include "btree/index-btree-internals.h"
#include "btree/index-btree-cache.h"
#include "btree/index-types.h"

int create_btree_file(FILE *data_fptr, FILE *btree_fptr);
int select_btree_data(FILE *data_fptr, BTREE **btree, FILE *btree_fptr, const JOGADOR where);

#endif