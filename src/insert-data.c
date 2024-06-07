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

    //setar status do cabeçalho pra inconsistente (0)
    fseek(data_fptr, 0, SEEK_SET);
    char status = '0';
    fwrite(&status, 1, 1, data_fptr);

    if (offset != -1){ //inserir no espaço encontrado
        
        //--ir para o offset encontrado, verificar o tam, escrever o novo registro e escrever $ no lixo

        //ir para o offset encontrado, pulando o char de removido
        fseek(data_fptr, offset+1, SEEK_CUR);
        int32_t rem_size = 0;
        fread(&rem_size, 4, 1, data_fptr);

        //escrevendo o novo registro
        fseek(data_fptr, offset, SEEK_CUR);
        append_reg(j_query, data_fptr);

        //PRECISO VERIFICAR SE O TAMANHO DO REGISTRO DEVE PERMANECER O ANTIGO OU ATUALIZO PARA O NOVO

        //se o reg_size for de tam diferente do antigo, escreve $ no lixo
        if(reg_size != rem_size){
            int diff = rem_size - reg_size;
            char trash = '$';
            
            for(int i=0; i < diff; i++){
                int32_t size_aux = 0; // Variável auxiliar para guardar o espaço ocupado pelos campos de string
                set_campo_str(trash, &size_aux, data_fptr);
            }
        }


    }else{ //inserir no final
        
    }

    //atualizar cabeçalho nroRegArq e nroRegRem, setar status pra consistente
    
    //se tiver id, atualizar arq de indice, inserindo ordenado de preferencia, escrever arquivo

    //lembrar de liberar arrays

}