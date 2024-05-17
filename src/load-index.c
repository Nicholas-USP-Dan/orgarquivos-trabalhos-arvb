#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "index-file.h"

#include "utils/data-file-utils.h"
#include "utils/cabecalho-utils.h"
#include "utils/campo-utils.h"

DYN_ARRAY* load_index(const char *index_filename){
    FILE *index_file_fptr = fopen(index_filename, "rb");
    if(index_file_fptr == NULL){
        return NULL;
    }

    if(!check_status(index_file_fptr)){
        errno = 1;
        return NULL;
    }

    DYN_ARRAY *array = initialize_dynarr();

    while(1){
        INDEX_REG reg;

        reg.index = get_campo32(index_file_fptr);
        reg.offset = get_campo64(index_file_fptr);

        if(feof(index_file_fptr)){
            break;
        }

        insert_last_dynarr(reg, &array);
    }

    fclose(index_file_fptr);
    return array;
}