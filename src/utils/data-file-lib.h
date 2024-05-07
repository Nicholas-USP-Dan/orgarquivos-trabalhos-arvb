#ifndef DATA_FILE_LIB_H
#define DATA_FILE_LIB_H

#include <stdint.h>
#include <stdio.h>

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

#endif