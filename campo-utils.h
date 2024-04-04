#ifndef CAMP_UTILS_H
#define CAMP_UTILS_H

#include <stdio.h>
#include <stdint.h>

/**
 * Atribui um char (string de 1 byte) a uma posicao no arquivo
 */
int set_campoc(const unsigned char val, const long offset, const int whence, FILE *fp);

/**
 * Atribui um inteiro de 32 bits (4 bytes) a uma posicao no arquivo
 */
int set_campo32(const int32_t val, const long offset, const int whence, FILE *fp);

/**
 * Atribui um inteiro de 64 bits (8 bytes) a uma posicao no arquivo
 */
int set_campo64(const int64_t val, const long offset, const int whence, FILE *fp);

/**
 * Recupera um char (string de 1 byte) em uma posicao no arquivo
 */
unsigned char get_campoc(const unsigned char errval, const long offset, const int whence, FILE *fp);

/**
 * Recupera um inteiro de 32 bits (4 bytes) a uma posicao no arquivo
 */
int32_t get_campo32(const int32_t errval, const long offset, const int whence, FILE *fp);

/**
 * Recupera um inteiro de 64 bits (8 bytes) a uma posicao no arquivo
 */
int64_t get_campo64(const int64_t errval, const long offset, const int whence, FILE *fp);

#endif