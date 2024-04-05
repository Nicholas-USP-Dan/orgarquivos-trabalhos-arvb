/** 
 * INTERFACE DE FUNCIONALIDADES DO CABECALHO
 * 
 * Este arquivo contem as declaracoes de funcoes dedicadas a manipulacao do cabecalho do arquivo de dados como especificado
 * no trabalho introdutorio da disciplina de Organizacao de Arquivos
 * 
 * Bibliotecas utilizadas por esta interface:
 * - stdio.h -> manipulacao de arquivos
 * - stdint.h -> tipos de inteiro com mesmo tamanho (em bytes) em varios sistemas
 * 
 * 
 */

#ifndef CABCL_UTILS_H
#define CABCL_UTILS_H

#include <stdio.h>
#include <stdint.h>

// Lista de offsets dos campos do cabecalho (campos estaticos)

#define STATUS_OFFSET 0
#define TOPO_OFFSET 1
#define PROXBYTE_OFFSET 9
#define NRO_REGARQ_OFFSET 17
#define NRO_REGREM_OFFSET 21
#define HEADER_END_OFFSET 25

/**
 * Inicializa um novo cabecalho em um arquivo binario
 * 
 * RETORNA: Retorna 0 quando a funcao termina com sucesso. Senao -1 e retornado
 */
int initialize_cabecalho(const unsigned char status, const int64_t topo, const int64_t prox_byte_offset, const int32_t nro_regarq, const int32_t nro_regrem, FILE *fp);

#endif