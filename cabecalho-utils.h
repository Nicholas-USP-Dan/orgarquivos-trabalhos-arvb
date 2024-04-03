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
 * Por razoes de deteccao de erro e simplicidade, as funcoes desta biblioteca nao consideram o caso do ponteiro do arquivo
 * estar nulo
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
int inicializar_cabecalho(FILE *fp);

/**
 * Recupera o valor do campo 'status' do cabecalho de um arquivo binario (criado por este programa)
 * 
 * TIPO: unsigned char -> string de 1 byte
 */
unsigned char get_status(FILE *fp);

/**
 * Atribui um novo valor para o campo 'status'
 * 
 * RETORNA: Retorna 0 quando a funcao termina com sucesso. Senao -1 e retornado
 */
int set_status(unsigned char b, FILE *fp);

/**
 * Recupera o valor do campo 'topo' do cabecalho de um arquivo binario (criado por este programa)
 * 
 * TIPO: int64_t -> int com 64 bits (8 bytes)
 */
int64_t get_topo(FILE *fp);

/**
 * Atribui um novo valor para o campo 'topo'
 * 
 * RETORNA: Retorna 0 quando a funcao termina com sucesso. Senao -1 e retornado
 */
int set_topo(int64_t topo, FILE *fp);

/**
 * Recupera o valor do campo 'proxByteOffset' do cabecalho de um arquivo binario (criado por este programa)
 * 
 * RETORNA: int64_t -> int com 64 bits (8 bytes)
 */
int64_t get_prox_byte_offset(FILE *fp);

/**
 * Atribui um novo valor para o campo 'proxByteOffset'
 * 
 * RETORNA: Retorna 0 quando a funcao termina com sucesso. Senao -1 e retornado
 */
int set_prox_byte_offset(int64_t prox_byte_offset, FILE *fp);

/**
 * Recupera o valor do campo 'nroRegArq' do cabecalho de um arquivo binario (criado por este programa)
 * 
 * RETORNA: int32_t -> int com 32 bits (4 bytes)
 */
int32_t get_nro_reg_arq(FILE *fp);

/**
 * Incrementa em n o valor do campo 'nroRegArq'
 * 
 * RETORNA: Retorna 0 quando a funcao termina com sucesso. Senao -1 e retornado
 */
int inc_nro_reg_arq(int32_t n, FILE *fp);

/**
 * Recupera o valor do campo 'nroRegRem' do cabecalho de um arquivo binario (criado por este programa)
 * 
 * RETORNA: int32_t -> int com 32 bits (4 bytes)
 */
int32_t get_nro_reg_rem(FILE *fp);

/**
 * Incrementa em n o valor do campo 'nroRegRem'
 * 
 * RETORNA: Retorna 0 quando a funcao termina com sucesso. Senao -1 e retornado
 */
int inc_nro_reg_rem(int32_t n, FILE *fp);

#endif