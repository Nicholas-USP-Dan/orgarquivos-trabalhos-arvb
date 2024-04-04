#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cabecalho-utils.h"
#include "campo-utils.h"

int initialize_cabecalho(const unsigned char status, const int64_t topo, const int64_t prox_byte_offset, const int32_t nro_regarq, const int32_t nro_regrem, FILE *fp){
    if(fseek(fp, 0, SEEK_SET) == -1){
        return -1;
    }
    
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

unsigned char get_status(FILE *fp){
    unsigned char res;

    if(fread(&res, 1, 1, fp) != sizeof(unsigned char)){
        return '0';
    }
    
    return res;
}

int set_status(unsigned char c, FILE *fp){
    return set_campoc(c, fp);
}

// int64_t get_topo(FILE *fp){
//     return get_campo64(-1, TOPO_OFFSET, SEEK_SET, fp);
// }

// int set_topo(int64_t topo, FILE *fp){
//     return set_campo64(topo, TOPO_OFFSET, SEEK_SET, fp);
// }

// int64_t get_prox_byte_offset(FILE *fp){
//     return get_campo64(0, PROXBYTE_OFFSET, SEEK_SET, fp);
// }

// int set_prox_byte_offset(int64_t prox_byte_offset, FILE *fp){
//     return set_campo64(prox_byte_offset, PROXBYTE_OFFSET, SEEK_SET, fp);
// }

// int32_t get_nro_reg_arq(FILE *fp){
//     return get_campo32(0, NRO_REGARQ_OFFSET, SEEK_SET, fp);
// }

// int inc_nro_reg_arq(int32_t n, FILE *fp){
//     int32_t new_val = get_nro_reg_arq(fp) + n;
//     return set_campo32(new_val, -4, SEEK_CUR, fp); /* A funcao get leva o ponteiro do arquivo logo apos o nro, entao o decremento de 4
//                                                     * leva o ponteiro de volta na posicao do nro
//                                                     */
// }

// int32_t get_nro_reg_rem(FILE *fp){
//     return get_campo32(0, NRO_REGREM_OFFSET, SEEK_SET, fp);
// }

// int inc_nro_reg_rem(int32_t n, FILE *fp){
//     int32_t new_val = get_nro_reg_rem(fp) + n;
//     return set_campo32(new_val, -4, SEEK_CUR, fp); /* A funcao get leva o ponteiro do arquivo logo apos o nro, entao o decremento de 4
//                                                     * leva o ponteiro de volta na posicao do nro
//                                                     */
// }