#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "index-file.h"

#include "adts/dyn-array.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"



int delete_data(FILE *data_fptr, FILE *index_fptr, JOGADOR j_search){
    
    if(j_search.id != jNil.id){
        //buscar por meio da funcionalidade 4 (suponho que por meio de indice)
        
    }else{
        //buscar pela funcionalidade 3 (busca normal)
    }

}
