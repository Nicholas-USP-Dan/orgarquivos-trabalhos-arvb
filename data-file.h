#include <stdio.h>
#include <stdint.h>

#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

/**
 * Funcao da funcionalidade 1
 * 
 * RETORNA: Retorna 0 qundo o arquivo é criado com sucesso, caso nao retorna -1
 */
int create_data_file_from_csv(const char *input_filename, const char *output_filename);

/**
 * Funcao da funcionalidade 2
 */
int select_fields_from_data_file(const char *input_filename);

/**
 * Funcao da funcionalidade 3
 */
int filter_data_file(const char *input_filename);