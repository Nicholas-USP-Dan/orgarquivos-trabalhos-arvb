/**
 * @file cabecalho-utils.c
 * @brief Source file para as funcionalidades do cabeçalho
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <stdint.h>

#include "cabecalho-utils.h"

int initialize_cabecalho(const unsigned char status, const int64_t topo, const int64_t prox_byte_offset, 
const int32_t nro_regarq, const int32_t nro_regrem, FILE *fp){
    // Escrita dos campos no arquivo binário
    fwrite(&status, 1, 1, fp);
    fwrite(&topo, 8, 1, fp);
    fwrite(&prox_byte_offset, 8, 1, fp);
    fwrite(&nro_regarq, 4, 1, fp);
    fwrite(&nro_regrem, 4, 1, fp);

    if(ftell(fp) != HEADER_END_OFFSET){ // Verifica se todos os bytes foram escritos
        return -1;
    }

    return 0;
}

int check_status(FILE *fp){
    char status;
    fread(&status, 1, 1, fp);
    return status == '1';
}