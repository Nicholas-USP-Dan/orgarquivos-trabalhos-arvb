#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "campo-utils.h"

unsigned char get_campoc(FILE *fp){
    unsigned char c = '\0';
    fread(&c, 1, 1, fp);

    return c;
}

int32_t get_campo32(FILE *fp){
    int32_t n = 0;
    fread(&n, 4, 1, fp);

    return n;
}

int64_t get_campo64(FILE *fp){
    int64_t n = 0;
    fread(&n, 8, 1, fp);

    return n;
}

char* get_campo_str(FILE *fp){
    int32_t size;
    fread(&size, 4, 1, fp);

    char *str = malloc(sizeof(char) * (size+1));
    fread(str, 1, size, fp);
    str[size] = '\0';

    return str;
}

int set_campoc(const unsigned char c, FILE *fp){
    if(fwrite(&c, 1, 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        return -1;
    }
    
    return 0;
}

int set_campo32(const int32_t val, FILE *fp){
    if(fwrite(&val, 4, 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        return -1;
    }
    
    return 0;
}

int set_campo64(const int64_t val, FILE *fp){
    if(fwrite(&val, 8, 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        return -1;
    }
    
    return 0;
}

int set_campo_str(const char *str, int32_t *campo_len, FILE *fp){
    int32_t len = strlen(str);
    if(fwrite(&len, 4, 1, fp) != 1){ // Tenta escrever os bytes na posicao colocada
        (*campo_len) = 0;
        return -1;
    }
    
    if(len > 0){
        if(fwrite(str, sizeof(char), len, fp) != len){ // Tenta escrever os bytes na posicao colocada
            (*campo_len) = 0;
            return -1;
        }
    }

    (*campo_len) = 4 + len;
    return 0;
}