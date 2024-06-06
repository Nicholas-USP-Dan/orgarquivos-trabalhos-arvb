#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"

int insert_data(FILE *data_fptr, FILE *index_fptr, JOGADOR j_query){
    //array contendo os índices do arquivo de dados
    DYN_ARRAY *index_arr = generate_index(data_fptr);

    //lista de registros removidos
    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

    int64_t offset = -1;

    // soma de bytes para a formação do registro
    int32_t lenNomeJog = strlen(j_query.nome);
    int32_t lenNac = strlen(j_query.nac);
    int32_t lenNomeClube = strlen(j_query.clube);

    // id = 1 | tamReg = 4 | prox = 8 | id = 4 | idade = 4 | tamNomeJog = 4 | nomeJog = var | tamNac = 4 | nac = var | tamNomeClube = 4 | nomeClube = var
    int32_t reg_size = 1 + 4 + 8 + 4 + 4 + 4 + lenNomeJog + 4 + lenNac + 4 + lenNomeClube;

    if (j_query.id != jNil.id){
        offset = find_space(reg_size, rem_list);
    }

    if (offset != -1){
        //inserir no espaço encontrado, lembrando de escrever $ no lixo que sobrar
    }else{
        //inserir no final
    }
    

}