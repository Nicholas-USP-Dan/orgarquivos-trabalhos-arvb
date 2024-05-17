/**
 * @file data-file.h
 * @brief Header file para as funcionalidades relacionadas aos arquivos binarios de dado.
 * 
 * Este arquivo contém as declarações das funcionalidades exigidas pelo Trabalho Prático Introdutório como especificado
 * na disciplina Organização de Arquivos.
 * 
 * @note Cada implementação está separada em um arquivo diferente para diminuir a linhas de código de cada arquivo.
 * 
 * @note Quando as funções das funcionalidades do projeto retornam -1, isso significa que houve um erro durante
 * a execução de uma função nativa do stdio, logo é possível extrair o erro obtido através da variável errno
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * @version 1.0
 */

#ifndef DATA_FILE_H
#define DATA_FILE_H

#include "adts/dyn-array.h"

/**
 * @brief Estrutura que contém os dados de um jogador, representa um registro no arquivo de dados.
 * 
 * @note As strings contidas dentro da estrutura terminam com '\0' para compatibilidade do processamento pelas funções
 * da biblioteca string.h
 */
struct _jogador {
    int32_t id; /**< id do jogador (32 bits ou 4 bytes) */
    int32_t idade; /**< idade do jogador (32 bits ou 4 bytes) */
    char *nome; /**< nome do jogador (string terminada em char nulo de tamanho variável)*/
    char *nac; /**< nacionalidade do jogador (string terminada em char nulo de tamanho variável)*/
    char *clube; /**< clube do jogador (string terminada em char nulo de tamanho variável)*/
};

/**
 * @brief Tipo da estrutura representando os dados de um jogador
 */
typedef struct _jogador JOGADOR;

/**
 * @brief Chamada da funcionalidade 1 - Cria um arquivo binario de dados a partir de um arquivo .csv
 * 
 * @param csv_fptr Ponteiro para o arquivo de entrada csv
 * @param data_fptr Ponteiro para o arquivo binário de saída
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 O A operação foi realizada com sucesso.
 */
int create_data_file(FILE *csv_fptr, FILE *data_fptr);

/**
 * @brief Chamada da funcionalidade 2 - Imprime todos os registros válidos (não removidos) de um arquivo binário de dados
 * 
 * @details A função lê a quantidade de registros válidos do arquivo (campo nroRegArq) para que ela possa parar a execução uma
 * vez que a quantidade de registros impressos for igual a quantidade de reg. válidos (útil para quando a quantidade de
 * registros remvido por alta).
 * 
 * @param data_fptr Ponteiro para o arquivo binário a ser lido.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int select_data(FILE *data_fptr);

/**
 * @brief Chamada da funcionalidade 3 - Filtra os registros válidos (não removidos) de um arquivo binário de dados
 * 
 * @details Esta função realiza n buscas, sendo que cada busca é identificada como "Busca X", onde X é a X° busca
 * realizada.
 * A função lê uma linha de busca com vários campos e realiza a busca conforme os critérios. Formado de uma linha de busca:
   @verbatim
   M nomeCampo1 valorCampo1 ... nomeCampoM valorCampoM (M pares de nome e valor)
   @endverbatim 
 * 
 * @param bin_filename Nome do arquivo binário de dados a ser lido.
 * 
 * @note Caso a busca contenha um critério de id, quando a função encontrar um jogador com este id, ele termina a busca.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int filter_data(FILE *data_fptr, JOGADOR j_query);

/**
 * @brief Chamada da funcionalidade 5 - Remove registros no arquivo binário de dados que cumprem um critério.
 * 
 * @param bin_filename Nome do arquivo binário de dados a ser operado
 * @param index_filename Nome do arquivo de índice.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int delete_data(const char *bin_filename, const char *index_filename);

/**
 * @brief Chamada da funcionalidade 6 - Insere um registro em um arquivo binário de dados.
 * 
 * @details Essa função utiliza-se do método Best Fit para encontrar espaços para ocupar; No método Best Fit
 * procura-se um espaço que tenha o menor espaço sobrando ao ser preenchido com o novo registro
 * 
 * @param bin_filename Nome do arquivo binário de dados a ser operado
 * @param index_filename Nome do arquivo de índice de saída.
 * 
 * @retval -1 Houve uma falha interna durante a operação.
 * @retval 0 A operação foi realizada com sucesso.
 */
int insert_data(const char *bin_filename, const char *index_filename); // Seria legal se a entrada pudesse ser feita no programa principal

#endif