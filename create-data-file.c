#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "data-file.h"

#include "cabecalho-utils.h"
#include "campo-utils.h"

#define BUFFER_SIZE 200

static char* get_campo_str(char **start_ptr, const char delim){
    if((*start_ptr) == NULL){
        return NULL;
    }
    
    char *token = malloc(BUFFER_SIZE * sizeof(char));

    char *end_token = strchr((*start_ptr), ',');
    if(end_token != NULL){
        strncpy(token, (*start_ptr), end_token-(*start_ptr));
        (*start_ptr) = end_token+1;
    }
    else{
        strcpy(token, (*start_ptr));
        (*start_ptr) = NULL;
    }

    return token;
}

/**
 * Função local que acrescenta um registro no arquivo de data binario
 */
static int add_reg_bfile(JOGADOR j, FILE *data_bfile_fptr){
    int32_t reg_size = 0;
    set_campoc('0', data_bfile_fptr); reg_size += 1;

    long reg_size_fpos = ftell(data_bfile_fptr);
    fseek(data_bfile_fptr, 4, SEEK_CUR); // Pular o campo do tamanho

    // O que fazer com o prox?? Garanto que o registro sempre vai ser adicionado no final?
    // No momento estou considerando que prox sempre fica nulo

    set_campo64(-1, data_bfile_fptr); reg_size += 8;
    set_campo32(j.id, data_bfile_fptr); reg_size += 4;
    set_campo32(j.idade, data_bfile_fptr); reg_size += 4;

    // Estou atribuindo o tamanho de cada campo em uma variavel para um futuro tratamento de erro

    int32_t size_aux_nome = set_campo_str(j.nome_jog, data_bfile_fptr);
    int32_t size_aux_nac = set_campo_str(j.nac, data_bfile_fptr);
    int32_t size_aux_clube = set_campo_str(j.nome_clube, data_bfile_fptr);

    reg_size += size_aux_nome + size_aux_nac + size_aux_clube;

    fseek(data_bfile_fptr, -(reg_size-5), SEEK_CUR);
    fwrite(&reg_size, 4, 1, data_bfile_fptr);

    fseek(data_bfile_fptr, reg_size-5, SEEK_CUR);

    return 0;
}

int create_data_file_from_csv(const char *input_filename, const char *output_filename){
    FILE *csv_data_fptr;
    if((csv_data_fptr = fopen(input_filename, "r")) == NULL){
        return -1;
    }

    FILE *data_bfile_fptr;
    if((data_bfile_fptr = fopen(output_filename, "wb")) == NULL){
        fclose(csv_data_fptr);
        return -1;
    }

    set_status('0', data_bfile_fptr);

    fseek(data_bfile_fptr, HEADER_END_OFFSET-1, SEEK_CUR);

    char columns[BUFFER_SIZE];
    fgets(columns, BUFFER_SIZE, csv_data_fptr);
    columns[strcspn(columns, "\n")] = 0;

    if(strcmp(columns, COLUMN_NAMES) != 0){ // As colunas do csv nao condiz
        fclose(csv_data_fptr);
        fclose(data_bfile_fptr);
        return -1; // Posso retornar 1 para indicar um erro lógico, não de saída
    }

    int32_t reg_count = 0;

    // while(1){
    //     char line_buff[BUFFER_SIZE];
    //     fgets(line_buff, sizeof(char) * BUFFER_SIZE, csv_data_fptr);

    //     if(feof(csv_data_fptr)){
    //         break;
    //     }

    //     line_buff[strcspn(line_buff, "\n")] = 0;

    //     char *start_ptr = line_buff;

    //     for(int i = 0; i < 5; i++){
    //         char *token = get_campo_str(&start_ptr, ',');
    //         printf("%s\n", token);
    //     }
        
    //     reg_count++;
    // }

    JOGADOR h = {
        .id = 80,
        .idade = 23,
        .nome_jog = "Armando",
        .nac = "",
        .nome_clube = "Santos"
    };

    add_reg_bfile(h ,data_bfile_fptr);

    fseek(data_bfile_fptr, 0, SEEK_SET);
    initialize_cabecalho('1', -1, HEADER_END_OFFSET, reg_count, 0, data_bfile_fptr);

    fclose(csv_data_fptr);
    fclose(data_bfile_fptr);
}