#ifndef INDEX_FILE_H
#define INDEX_FILE_H

#include "adts/dyn-array.h"

/**
 * @brief Chamada da funcionalidade 4 - Cria um arquivo de índice primário de um arquivo binário de dados.
 * 
 * @param bin_filename Nome do arquivo binário de dados a ser referenciado.
 * @param index_filename Nome do arquivo de índice de saída.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 * @retval 1 Houve algum erro lógico durante a operação.
 */
int create_index(const char *bin_filename, const char *index_filename);

DYN_SORTED_ARRAY* load_index(const char *index_filename);

int update_index(DYN_SORTED_ARRAY **index_array);

#endif