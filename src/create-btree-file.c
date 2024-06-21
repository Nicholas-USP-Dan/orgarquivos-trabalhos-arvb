#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "btree-func.h"

#include "btree/index-btree.h"
#include "btree/index-types.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

int create_btree_file(FILE *data_fptr, FILE *btree_fptr){
    BTREE *btree = initialize_btree();

    // Pular o cabecalho
    fseek(data_fptr, HEADER_END_OFFSET, SEEK_SET);

    // Lê registros até ler todos os registros válidos
    while(1){
        int64_t reg_offset = ftell(data_fptr);
        unsigned char rem = get_campoc(data_fptr);

        if(feof(data_fptr)){
            break;
        }

        int32_t reg_size = get_campo32(data_fptr);

        // Registro esta removido, mover para o proximo
        if(rem == '1'){
            fseek(data_fptr, reg_size-5, SEEK_CUR);
            continue;
        }

        // Pular tamanhoRegistro e Prox
        fseek(data_fptr, 8, SEEK_CUR);

        JOGADOR j = read_jogador_data(data_fptr);

        // Adiciona o registro na árvore-b
        if(insert_btree((INDEX_REG){j.id, reg_offset}, &btree, btree_fptr) == -1){
            return -1;
        }

        free_jogador(&j);

        fseek(data_fptr, reg_offset + reg_size, SEEK_SET);
    }

    set_btree_cabecalho('1', &btree, btree_fptr);

    dump_btree_cache(&btree, btree_fptr);

    clear_btree(&btree);

    return 0;
}