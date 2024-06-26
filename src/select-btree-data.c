#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

#include "adts/dyn-array.h"
#include "data-file.h"

//#include "index-file.h"
#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"
#include "utils/removed-list.h"

#include "btree-func.h"
#include "btree/index-btree.h"
#include "btree/index-btree-internals.h"
#include "btree/index-btree-cache.h"
#include "btree/index-types.h"

int select_btree_data(FILE *data_fptr, BTREE **btree, FILE *btree_fptr, const JOGADOR where){
    //caso tenhamos o campo id, procuraremos o registro pelo índice na arvore-b
    if(where.id != jNil.id){
        int64_t offset = search_btree(where.id, btree, btree_fptr);

        if(offset == -1){
            printf("Registro inexistente.\n\n");
        }

        fseek(data_fptr, offset + 13, SEEK_SET);
        JOGADOR j = read_jogador_data(data_fptr);

        print_jogador(j);
        printf("\n");
    }else{
        select_data(data_fptr, where);
    }

    return 0;
}