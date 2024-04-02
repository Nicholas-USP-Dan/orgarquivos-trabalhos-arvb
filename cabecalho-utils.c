#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "cabecalho-utils.h"

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

/**
 * Tenta realizar um fseek com os argumentos passados na funcao
 */
#define try_seek(fp, offset, whence) (fp != NULL && fseek(fp, offset, whence) != -1)

// static int try_seek(FILE *fp, long offset, int whence){
//     return fp != NULL && fseek(fp, offset, whence) != -1;
// }

int inicializar_cabecalho(FILE *fp){
    if(!try_seek(fp, 0, SEEK_SET)){
        return -1;
    }

    unsigned char zchar = DEFAULT_STATUS;
    int64_t minus = DEFAULT_TOPO;
    int64_t z64 = DEFAULT_PROXBYTE;
    int32_t z32 = DEFAULT_NRO_REGARQ;
    
    fwrite(&zchar, sizeof(unsigned char), 1, fp); // status
    fwrite(&minus, sizeof(int64_t), 1, fp); // topo
    fwrite(&z64, sizeof(int64_t), 1, fp); // proxByteOffset
    fwrite(&z32, sizeof(int32_t), 1, fp); // nroRegArq
    fwrite(&z32, sizeof(int32_t), 1, fp); // nroRegRem

    if(ftell(fp) != HEADER_END_OFFSET){
        return -1;
    }

    return 0;
}

unsigned char get_status(FILE *fp){
    if(!try_seek(fp, STATUS_OFFSET, SEEK_SET)){
        return '$';
    }

    unsigned char res = '$';
    fread(&res, sizeof(unsigned char), 1, fp);
    
    return res;
}

int set_status(unsigned char b, FILE *fp){
    if(!try_seek(fp, STATUS_OFFSET, SEEK_SET)){
        return -1;
    }

    if(fwrite(&b, sizeof(unsigned char), 1, fp) != 1){
        return -1;
    }
    
    return 0;
}

int64_t get_topo(FILE *fp){
    if(!try_seek(fp, TOPO_OFFSET, SEEK_SET)){
        return -1;
    }

    int64_t topo = -1;
    fread(&topo, sizeof(int64_t), 1, fp);
    
    return topo;
}

int update_topo(int64_t topo, FILE *fp){
    if(!try_seek(fp, TOPO_OFFSET, SEEK_SET)){
        return -1;
    }

    if(fwrite(&topo, sizeof(int64_t), 1, fp) != 1){
        return -1;
    }
    
    return 0;
}