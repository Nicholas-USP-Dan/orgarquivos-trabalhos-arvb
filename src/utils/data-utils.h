/**
 * @file data-utils.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <stdint.h>
#include <stdio.h>

#include "../data-file.h"

#define IDMASK 0x01
#define IDADEMASK 0x02
#define NOMEMASK 0x04
#define NACMASK 0x08
#define CLUBEMASK 0x10

/**
 * @brief Nome das colunas como registrado nas especificações do projeto
 * 
 */
#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

/**
 * @brief Instância de um jogador vazio/nulo, representa uma busca "vazia"
 */
extern const JOGADOR jNil;

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
void print_jogador(const JOGADOR j);

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
 * @note Foi usado uma bitmask para otimizar o processo de filtragem.
 * 
 * @return [JOGADOR] Retorna um objeto do tipo JOGADOR guardando os dados dos campos a serem filtrados;
 * os campos não procurados são atribuídos com o mesmo valor do jogador nulo (jNil).
 */
JOGADOR read_query();

int pass_where(const JOGADOR j, const JOGADOR where);

// int delete_reg(FILE *data_fptr, REM_LIST **rem_list, DYN_ARRAY **index_arr);

#endif