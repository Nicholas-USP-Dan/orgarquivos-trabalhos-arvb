#ifndef REMOVED_LIST_H
#define REMOVED_LIST_H

#include <stdio.h>
#include <stdint.h>

#include "../adts/dyn-array.h"

struct _rem_el {
    int64_t offset;
    int32_t tam;
};

typedef struct _rem_el REM_EL;

enum alloc_met {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

struct _rem_list {
    DYN_ARRAY *arr;
    enum alloc_met met;
};

typedef struct _rem_list REM_LIST;

REM_LIST* initialize_rem_list(const enum alloc_met met);

void clear_rem_list(REM_LIST **list);

/**
 * @brief Carrega a lista de removidos de um arquivo de dados
 * 
 * @param data_fptr Arquivo de dados da lista
 * @param met Metodo de reaproveitamento de espaço
 * 
 * @returns [REM_LIST*] Ponteiro para uma nova lista de removidos
*/
REM_LIST* load_rem_list(FILE *data_fptr, const enum alloc_met met);

/**
 * @brief Acha um espaço livre em uma lista de registros removidos de acordo com o método definido; 
 * caso não haja espaço disponível retorna -1
 * 
 * @param tam Tamanho do registro procurado
 * @param list Referência para o ponteiro da lista de remoção
 * 
 * @returns [int64_t] Offset do espaço livre disponível; caso não haja, retorna -1
*/
int64_t find_space(const int32_t tam, REM_LIST **list);

int write_rem_list(FILE *data_fptr, REM_LIST **list);

void sort_rem_list(REM_LIST **list);

int add_rem_list(FILE *data_fptr, REM_LIST **rem_list, int offset);

#endif