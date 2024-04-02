#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cabecalho-utils.h"
#include "campo-utils.h"

// Lista de offsets dos campos do cabecalho (campos estaticos)

#define STATUS_OFFSET 0
#define TOPO_OFFSET 1
#define PROXBYTE_OFFSET 9
#define NRO_REGARQ_OFFSET 17
#define NRO_REGREM_OFFSET 21
#define HEADER_END_OFFSET 25


// Valores padrao

#define DEFAULT_STATUS '0'
#define DEFAULT_TOPO -1
#define DEFAULT_PROXBYTE 0
#define DEFAULT_NRO_REGARQ 0
#define DEFAULT_NRO_REGREM DEFAULT_NRO_REGARQ

int inicializar_cabecalho(FILE *fp){
    if(fseek(fp, 0, SEEK_SET) == -1){
        return -1;
    }

    // Atribuicao dos valores padroes para o arquivo
    
    unsigned char zchar = DEFAULT_STATUS;
    int64_t minus = DEFAULT_TOPO;
    int64_t z64 = DEFAULT_PROXBYTE;
    int32_t z32 = DEFAULT_NRO_REGARQ;
    
    fwrite(&zchar, sizeof(unsigned char), 1, fp); // status
    fwrite(&minus, sizeof(int64_t), 1, fp); // topo
    fwrite(&z64, sizeof(int64_t), 1, fp); // proxByteOffset
    fwrite(&z32, sizeof(int32_t), 1, fp); // nroRegArq
    fwrite(&z32, sizeof(int32_t), 1, fp); // nroRegRem

    if(ftell(fp) != HEADER_END_OFFSET){ // Verifica se todos os bytes foram escritos
        return -1;
    }

    return 0;
}

unsigned char get_status(FILE *fp){
    return get_campoc('$', STATUS_OFFSET, SEEK_SET, fp);
}

int set_status(unsigned char b, FILE *fp){
    return set_campoc(b, STATUS_OFFSET, SEEK_SET, fp);
}

int64_t get_topo(FILE *fp){
    return get_campo64(-1, TOPO_OFFSET, SEEK_SET, fp);
}

int set_topo(int64_t topo, FILE *fp){
    return set_campo64(topo, TOPO_OFFSET, SEEK_SET, fp);
}

int64_t get_prox_byte_offset(FILE *fp){
    return get_campo64(0, PROXBYTE_OFFSET, SEEK_SET, fp);
}

int set_prox_byte_offset(int64_t prox_byte_offset, FILE *fp){
    return set_campo64(prox_byte_offset, PROXBYTE_OFFSET, SEEK_SET, fp);
}

int32_t get_nro_reg_arq(FILE *fp){
    return get_campo32(0, NRO_REGARQ_OFFSET, SEEK_SET, fp);
}

int inc_nro_reg_arq(int32_t n, FILE *fp){
    int32_t new_val = get_nro_reg_arq(fp) + n;
    return set_campo32(new_val, -4, SEEK_CUR, fp); /* A funcao get leva o ponteiro do arquivo logo apos o nro, entao o decremento de 4
                                                    * leva o ponteiro de volta na posicao do nro
                                                    */
}

int32_t get_nro_reg_rem(FILE *fp){
    return get_campo32(0, NRO_REGREM_OFFSET, SEEK_SET, fp);
}

int inc_nro_reg_rem(int32_t n, FILE *fp){
    int32_t new_val = get_nro_reg_rem(fp) + n;
    return set_campo32(new_val, -4, SEEK_CUR, fp); /* A funcao get leva o ponteiro do arquivo logo apos o nro, entao o decremento de 4
                                                    * leva o ponteiro de volta na posicao do nro
                                                    */
}