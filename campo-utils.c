#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "campo-utils.h"

int set_campoc(const unsigned char val, const long offset, const int whence, FILE *fp){
    if(fseek(fp, offset, whence) == -1){
        return -1;
    }

    if(fwrite(&val, sizeof(unsigned char), 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        return -1;
    }
    
    return 0;
}

int set_campo32(const int32_t val, const long offset, const int whence, FILE *fp){
    if(fseek(fp, offset, whence) == -1){
        return -1;
    }

    if(fwrite(&val, sizeof(int32_t), 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        return -1;
    }
    
    return 0;
}

int set_campo64(const int64_t val, const long offset, const int whence, FILE *fp){
    if(fseek(fp, offset, whence) == -1){
        return -1;
    }

    if(fwrite(&val, sizeof(int64_t), 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        return -1;
    }
    
    return 0;
}

unsigned char get_campoc(const unsigned char errval, const long offset, const int whence, FILE *fp){
    if(fseek(fp, offset, whence) == -1){
        return errval;
    }

    unsigned char res = errval;
    fread(&res, sizeof(unsigned char), 1, fp); // Tenta ler os bytes na posicao colocada
    
    return res;
}

int32_t get_campo32(const int32_t errval, const long offset, const int whence, FILE *fp){
    if(fseek(fp, offset, whence) == -1){
        return errval;
    }

    int32_t res = errval;
    fread(&res, sizeof(int32_t), 1, fp); // Tenta ler os bytes na posicao colocada
    
    return res;
}

int64_t get_campo64(const int64_t errval, const long offset, const int whence, FILE *fp){
    if(fseek(fp, offset, whence) == -1){
        return errval;
    }

    int64_t res = errval;
    fread(&res, sizeof(int64_t), 1, fp); // Tenta ler os bytes na posicao colocada
    
    return res;
}