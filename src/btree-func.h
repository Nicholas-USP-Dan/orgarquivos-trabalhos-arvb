#ifndef BTREE_FUNC_H
#define BTREE_FUNC_H

#include <stdio.h>

#include "btree/index-btree.h"
#include "utils/removed-list.h"
#include "data-file.h"

int create_btree_file(FILE *data_fptr, FILE *btree_fptr);

int select_btree_data(FILE *data_fptr, BTREE **btree, FILE *btree_fptr, const JOGADOR where);

int insert_data_btree(FILE *data_fptr, const JOGADOR j, int *quant_ins, REM_LIST **rem_list, BTREE **btree, FILE *btree_fptr);

#endif