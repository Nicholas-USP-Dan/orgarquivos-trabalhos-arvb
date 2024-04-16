/**
 * @file data-file.h
 * @brief Header file para as funcionalidades relacionadas aos arquivos binarios de dado.
 * 
 * Este arquivo contém as declarações das funcionalidades exigidas pelo Trabalho Prático Introdutório como especificado
 * na disciplina Organização de Arquivos.
 * 
 * @note Cada implementação está separada em um arquivo diferente para diminuir a linhas de código de cada arquivo,
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * @version 1.0
 */

#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <stdint.h>

#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube" /**< Nome das colunas como registrado no Trabalho Introdutório
de Organização de Arquivos */

/**
 * @brief Estrutura que contém os dados de um jogador, pode representar um registro no arquivo de dados.
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
 * 
 */
typedef struct _jogador JOGADOR;

/**
 * Limpa a memria alocada por um jogador (strings) 
 */
void free_jogador(JOGADOR *j);

/**
 * Funcao que imprime um jogador de acordo com os requisitos do projeto 
 */
void print_jogador(JOGADOR j);

/**
 * Le um registro em fptr e atribui os valores na variavel JOGADOR
 */
JOGADOR read_jogador_data(FILE *fptr);

/**
 * Funcao da funcionalidade 1 - Criar um arquivo binario de dados a partir de um arquivo de dados .csv
 * 
 * RETORNA: Retorna 0 qundo o arquivo é criado com sucesso, caso nao retorna -1
 */
int create_data_file_from_csv(const char *input_filename, const char *output_filename);

/**
 * Funcao da funcionalidade 2 - Imprime todos os registros validos (nao removidos) de um arquivo binario de dados
 */
int select_data_file(const char *input_filename);

/**
 * Funcao da funcionalidade 3 - Filtra os registros validos (nao removidos) de um arquivo binario de dados
 * 
 * Se realiza n pesquisar, cada uma sendo rotulada como Busca x
 */
int filter_data_file(const int n, const char *input_filename);

#endif