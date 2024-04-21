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

#include <stdint.h>

/**
 * @brief Nome das colunas como registrado nas especificações do projeto
 * 
 */
#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

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
 * @brief Função que limpa a memória alocada por um jogador (alocado pelos campos de string)
 * 
 * @param j Referência ao jogador a ter a memória limpa
 */
void free_jogador(JOGADOR *j);

/**
 * @brief Função que imprime, conforme os requisitos do projeto, os dados de um jogador
 * 
 * @details É imprimido o nome do jogador, a nacionalidade dele e o seu clube, ou seja, todas as strings do jogador.
 * 
 * @verbatim
    ----- EX DE SAÍDA -----
    Nome do Jogador: A. SMITH
    Nacionalidade do Jogador: ENGLAND
    Clube do jogador: E. UNAL
   @endverbatim
 * 
 * @param j Jogador a ser imprimido
 */
void print_jogador(JOGADOR j);

/**
 * @brief Função que lê um registro no arquivo binário e retorna um objeto do tipo JOGADOR 
 * com os dados obtidos pela função.
 * 
 * @param fptr Ponteiro para o arquivo binário de dados.
 * 
 * @note Esta função considera que o ponteiro do arquivo está apontada para o campo id, logo os campos 
 * "removido", "tamanhoRegistro" e "Prox" devem ser pulados antes desta função ser chamada.
 */
JOGADOR read_jogador_data(FILE *fptr);

/**
 * @brief Chamada da funcionalidade 1 - Cria um arquivo binario de dados a partir de um arquivo .csv
 * 
 * @param input_filename Nome do arquivo de entrada .csv.
 * @param output_filename Nome do arquivo binário de saída.
 * 
 * @retval -1 Houve uma falha interna durante a geração do arquivo binário.
 * @retval 0 O arquivo binário foi criado com sucesso.
 * @retval 1 Houve algum erro lógico durante a geração do arquivo binário.
 */
int create_data_file_from_csv(const char *input_filename, const char *output_filename);

/**
 * @brief Chamada da funcionalidade 2 - Imprime todos os registros válidos (não removidos) de um arquivo binário de dados
 * 
 * @details A função lê a quantidade de registros válidos do arquivo (campo nroRegArq) para que ela possa parar a execução uma
 * vez que a quantidade de registros impressos for igual a quantidade de reg. válidos (útil para quando a quantidade de
 * registros remvido por alta).
 * 
 * @param input_filename Nome do arquivo binário de dados.
 * 
 * @retval -1 Houve uma falha interna durante a impressão dos registros.
 * @retval 0 Os registros foram impressos com sucesso.
 * @retval 1 Houve algum erro lógico durante a impressão dos registros.
 */
int select_data_file(const char *input_filename);

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
 * @param n Quantidade de buscas a serem realizadas.
 * @param input_filename Nome do arquivo binário de dados.
 * 
 * @note Caso a busca contenha um critério de id, quando a função encontrar um jogador com este id, ele termina a busca.
 * 
 * @retval -1 Houve uma falha interna durante as filtragens do arquivo.
 * @retval 0 Os registros foram impressos com sucesso.
 * @retval 1 Houve algum erro lógico durante as filtragens do arquivo.
 */
int filter_data_file(const int n, const char *input_filename);

#endif