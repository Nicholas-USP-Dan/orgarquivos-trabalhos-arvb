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

#include "utils/data-file-lib.h"

/**
 * @brief Nome das colunas como registrado nas especificações do projeto
 * 
 */
#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

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