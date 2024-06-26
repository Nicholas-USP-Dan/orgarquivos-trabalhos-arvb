/**
 * @file insert-data.c
 * @brief Implementação da funcionalidade 10 - Inserção de um registro em um arquivo de dados e uma árvore-b (estrutura de indexação)
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 3.0
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "btree/index-btree.h"
#include "btree/index-types.h"
#include "data-file.h"

#include "utils/data-utils.h"
#include "utils/campo-utils.h"
#include "utils/cabecalho-utils.h"
#include "utils/removed-list.h"

int insert_data_btree(FILE *data_fptr, const JOGADOR j, int *quant_ins, REM_LIST **rem_list, BTREE **btree, FILE *btree_fptr) {
    int ret = -1;

    // Settar status do cabeçalho pra inconsistente (0)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);

    set_campoc('0', data_fptr);

    // Settar status da árvore-b (junto com o cabeçalho inteiro) como instável
    // Como a manipulação do arquivo ocorre em páginas, esta chamada não requer
    // mais seeks do que settar apenas o status
    // 
    // NOTE: supõe-se que o cabeçalho da árvore-b já tenha sido lido corretamente
    set_btree_cabecalho('0', btree, btree_fptr);

    // Ler o campo "proxByteOffset"
    fseek(data_fptr, PROXBYTE_OFFSET, SEEK_SET);
    int64_t proxbyte_offset = get_campo64(data_fptr);

    // Verifica se na lista de removidos tem algum espaço que caiba o novo registro
    // Se couber utiliza ele, se não, utiliza o proxByteOffset
    int32_t reg_size = get_reg_size(j);
    REM_EL rem_el = find_space(reg_size, rem_list);

    int64_t offset = rem_el.offset != -1 ? rem_el.offset : proxbyte_offset;

    if(rem_el.offset == -1){
        // Não houve espaço na lista de removidos => inserir no final do arquivo
        fseek(data_fptr, proxbyte_offset, SEEK_SET);

        // Escrevendo o novo registro
        append_reg(j, data_fptr);

        // Atualizar o valor do proxbyte_offset
        proxbyte_offset = ftell(data_fptr);
        fseek(data_fptr, PROXBYTE_OFFSET, SEEK_SET);
        set_campo64(proxbyte_offset, data_fptr);
    }
    else { // Inserir no espaço encontrado
        // Ir para o offset encontrado, verificar o tam, escrever o novo registro e escrever $ no lixo

        // Ir para o offset encontrado, pulando o char de removido
        fseek(data_fptr, rem_el.offset+1, SEEK_SET);
        int32_t rem_size = get_campo32(data_fptr);

        // Escrevendo o novo registro
        fseek(data_fptr, rem_el.offset, SEEK_SET);
        append_reg(j, data_fptr);

        int32_t aux_size = reg_size;
        while(aux_size < rem_size){
            char trash = '$';
            set_campoc(trash, data_fptr);
            aux_size++;
        }
        
        // Alterando o tamanho do registro para manter o antigo (do registro removido)
        fseek(data_fptr, rem_el.offset+1, SEEK_SET);
        // fwrite(&rem_size, 4, 1, data_fptr);
        set_campo32(rem_size, data_fptr);
    }

    insert_btree((INDEX_REG){.index=j.id, .offset = offset}, btree, btree_fptr);

    (*quant_ins)++;

    ret = 0;
    
    return ret;
}