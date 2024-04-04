#include <stdio.h>
#include <stdint.h>

#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

typedef struct _jogador {
    int32_t id;
    int32_t idade;
    char *nome_jog;
    char *nac;
    char *nome_clube;
} JOGADOR;

/**
 * Funcao da funcionalidade 1
 * 
 * RETORNA: Retorna 0 qundo o arquivo é criado com sucesso, caso nao retorna -1
 */
int create_data_file_from_csv(const char *input_filename, const char *output_filename);

/**
 * Funcao da funcionalidade 2
 */
int select_data_file(const char *input_filename);

/**
 * Funcao da funcionalidade 3
 */
int filter_data_file(const char *input_filename);