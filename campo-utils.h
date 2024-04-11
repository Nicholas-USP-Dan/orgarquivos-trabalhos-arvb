#ifndef CAMP_UTILS_H
#define CAMP_UTILS_H

#include <stdio.h>
#include <stdint.h>

/**
 * Tamanho alocado aos buffers de chars
 */
#define BUFFER_SIZE 200

/**
 * Offset logo apos os campos estaticos de um registro
 */
#define STATIC_REG_END_OFFSET 25

unsigned char get_campoc(FILE *fp);

int32_t get_campo32(FILE *fp);

int64_t get_campo64(FILE *fp);

char* get_campo_str(FILE *fp);

/**
 * Atribui um char (string de 1 byte) no espaco apontado no arquivo
 */
int set_campoc(const unsigned char c, FILE *fp);

/**
 * Atribui um inteiro de 32 bits (4 bytes) no espaco apontado no arquivo
 */
int set_campo32(const int32_t val, FILE *fp);

/**
 * Atribui um inteiro de 64 bits (8 bytes) no espaco apontado no arquivo
 */
int set_campo64(const int64_t val, FILE *fp);

/**
 * Atribui uma string e seu tamanho no espaco apontado no arquivo
 * 
 * RETORNA: Retorna a quantidade de bytes ocupados pelo campo (incluindo o campo de tamanho)
 * Retorna -1 caso ocorra algum erro
 */
int set_campo_str(const char *str, int32_t *campo_len, FILE *fp);

#endif