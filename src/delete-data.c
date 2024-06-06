#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

int delete_data(FILE *data_fptr, JOGADOR where, REM_LIST **rem_list, DYN_ARRAY **index_arr){
    if(where.id != jNil.id){
            //buscar por meio da funcionalidade 4 (suponho que por meio de indice)
            ret = delete_index(data_fptr, where, rem_list, index_arr);
        }else{
            //buscar pela funcionalidade 3 (busca normal)
            //mas nao da pra usar func3 direto pq senao vai pedir input dnv
            ret = func3();
        }
}