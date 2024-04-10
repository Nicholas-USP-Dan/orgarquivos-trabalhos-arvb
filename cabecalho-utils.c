#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cabecalho-utils.h"

int initialize_cabecalho(const unsigned char status, const int64_t topo, const int64_t prox_byte_offset, const int32_t nro_regarq, const int32_t nro_regrem, FILE *fp){
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