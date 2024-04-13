#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "funcoes_fornecidas.h"
#include "cabecalho-utils.h"
#include "campo-utils.h"

int select_data_file(const char *input_filename){
    FILE *fptr = fopen(input_filename, "rb")

    if(fptr == NULL){
        printf("Falha no processamento do arquivo.")
        return -1;
    }

    // Verificação do status no cabeçalho
    if(!check_status(fptr)){
        return -1;
    }
}