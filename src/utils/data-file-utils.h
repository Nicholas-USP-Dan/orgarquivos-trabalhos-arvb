#ifndef DATA_FILE_LIB_H
#define DATA_FILE_LIB_H

#include <stdint.h>
#include <stdio.h>

#define IDMASK 0x01
#define IDADEMASK 0x02
#define NOMEMASK 0x04
#define NACMASK 0x08
#define CLUBEMASK 0x10

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
 * @brief Instância de um jogador vazio/nulo, representa uma busca "vazia"
 */
extern const JOGADOR jNil;
/**
 * @brief Estrutura que contém os dados de um registro de índice
 */
struct _index_reg {
    int32_t index;
    int64_t offset;
};

/**
 * @brief Tipo da estrutura representando os dados de um registro de índice
 * 
 */
typedef struct _index_reg INDEX_REG;

/**
 * @brief Array com todos os nomes dde campos válidos para realizar a filtragem
 */
extern const char *CAMPO_LIST[5];;

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
 * @brief Lê uma linha (do stdin) com os campos da busca e retorna um jogador guardando esses dados.
 * 
 * @param [out] mask Bitmask representando os campos para serem filtrados; esses campos têm 
 * seus bits representantes marcados como 1
 * 
 * @note Foi usado uma bitmask para otimizar o processo de filtragem.
 * 
 * @return [JOGADOR] Retorna um objeto do tipo JOGADOR guardando os dados dos campos a serem filtrados;
 * os campos não procurados são atribuídos com o mesmo valor do jogador nulo (jNil).
 */
JOGADOR read_query(unsigned int *mask);

#endif