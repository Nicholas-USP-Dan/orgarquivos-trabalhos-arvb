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

    int ret = -1;

    //setar status do cabeçalho pra inconsistente (0)
    fseek(data_fptr, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, 1, 1, data_fptr);

    if (offset != -1){ //inserir no espaço encontrado
        
        //--ir para o offset encontrado, verificar o tam, escrever o novo registro e escrever $ no lixo

        //ir para o offset encontrado, pulando o char de removido
        fseek(data_fptr, offset+1, SEEK_SET);
        int32_t rem_size = 0;
        fread(&rem_size, 4, 1, data_fptr);

        //escrevendo o novo registro
        fseek(data_fptr, offset, SEEK_SET);
        append_reg(j_query, data_fptr);

        //se o reg_size for de tam diferente do antigo, escreve $ no lixo
        if(reg_size != rem_size){
            int diff = rem_size - reg_size;
            char trash = '$';
            
            for(int i=0; i < diff; i++){
                int32_t size_aux = 0; // Variável auxiliar para guardar o espaço ocupado pelos campos de string
                set_campo_str(trash, &size_aux, data_fptr);
            }
        }

        //alterando o tamanho do registro para manter o antigo (do registro removido)
        fseek(data_fptr, offset+1, SEEK_SET);
        fwrite(&rem_size, 4, 1, data_fptr);

        // ----- !!!!
        //OUTRO PROBLEMA PRA VERIFICAR - o byteoffset  do proximo registro logicamente removível
        //possivelmente eu tenha que ligar o byteoffset do proximo reg ao reg anterior
        // ----- !!!!

    }else{ //inserir no final
        fseek(data_fptr, 0, SEEK_END);

        //escrevendo o novo registro
        offset = ftell(data_fptr);
        append_reg(j_query, data_fptr);
        
    }

    //atualizar cabeçalho nroRegArq e nroRegRem, setar status pra consistente

    //status consistente
    fseek(data_fptr, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, 1, 1, data_fptr);

    //altera nroRegArq e nroRegRem

    int64_t aux_8bytes;
    int32_t aux_4bytes;
    int32_t nro_regarq;
    int32_t nro_regrem;

    //lendo auxiliares pra nao dar outro fseek
    fread(&aux_8bytes, 8, 1, data_fptr); // lendo o topo
    fread(&aux_8bytes, 8, 1, data_fptr); // lendo o prox_byte_offset

    //guardando valores de regArq e regRem para editá-los
    fread(&nro_regarq, 4, 1, data_fptr); // lendo o nro_regarq
    fread(&nro_regrem, 4, 1, data_fptr); // lendo o nro_regrem

    size_t currOffset = ftell(data_fptr);

    fseek(data_fptr, currOffset-8, SEEK_CUR);

    nro_regarq = nro_regarq + 1;
    nro_regrem = nro_regrem - 1;

    fwrite(&nro_regarq, 4, 1, data_fptr);
    fwrite(&nro_regrem, 4, 1, data_fptr);

    if(ftell(data_fptr) != HEADER_END_OFFSET){ // Verifica se todos os bytes foram escritos
        return -1;
    }
    
    //se tiver id, atualizar arq de indice, inserindo ordenado + escrever arquivo

    if (j_query.id != jNil.id){

        //adicionando indice novo no array
        INDEX_REG *aux_temp = malloc(sizeof(INDEX_REG));
        aux_temp->index = j.id;
        aux_temp->offset = offset;

        insert_ord_dynarr(aux_temp, &index_arr);

        write_index(&index_arr, index_fptr);

    }

    //lembrar de liberar arrays
    clear_dynarr(&index_arr);
    clear_rem_list(&rem_list);

    ret = 0;
    
    return ret;

}