#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "data-file.h"

#include "cabecalho-utils.h"
#include "campo-utils.h"

/**
 * Remove o caracter newline ('\n') de uma string
 */
#define TRIM_NEWLINE(str) str[strcspn(str, "\n")] = 0

/**
 * Implementacao semelhante da funcao strtok com leitura de campos vazios
 */
static char* get_token_str(char **start_ptr, const char delim){
    if((*start_ptr) == NULL){
        return NULL;
    }
    
    char *token = calloc(BUFFER_SIZE/2, sizeof(char));

    char *end_token = strchr((*start_ptr), delim);
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
 * Processa uma linha extraida de um arquivo .csv (string com tokens separadas por ',') e 
 * retorna uma variavel do tipo JOGADOR com os dados preenchidos
 */
static JOGADOR process_jogador(char *line){
    JOGADOR j_out;
    char *line_ptr = line;
    int32_t len;

    // ADICIONAR FUNCIONALIDADE PARA LER .CSV

    char *id_str = get_token_str(&line_ptr, ',');
    // Poderia verificar tambem, mas sei que sempre havera um id
    sscanf(id_str, "%" PRId32, &j_out.id);
    free(id_str);
    
    char *idade_str = get_token_str(&line_ptr, ',');
    if(strcmp(idade_str, "") != 0){
        sscanf(idade_str, "%" PRId32, &j_out.idade);
    }
    else{
        j_out.idade = -1;
    }
    free(idade_str);

    char *nome_str = get_token_str(&line_ptr, ',');
    len = strlen(nome_str);
    j_out.nome = (char *)malloc(len+1);
    strcpy(j_out.nome, nome_str);
    free(nome_str);

    char *nac_str = get_token_str(&line_ptr, ',');
    len = strlen(nac_str);
    j_out.nac = (char *)malloc(len+1);
    strcpy(j_out.nac, nac_str);
    free(nac_str);

    char *clube_str = get_token_str(&line_ptr, ',');
    len = strlen(clube_str);
    j_out.clube = (char *)malloc(len+1);
    strcpy(j_out.clube, clube_str);
    free(clube_str);

    return j_out;
}

/**
 * Função local que acrescenta um registro no arquivo de data binario
 */
static int add_reg_bfile(const JOGADOR j, FILE *data_bfile_fptr){
    int32_t reg_size = 0;
    set_campoc('0', data_bfile_fptr); reg_size += 1;

    long reg_size_fpos = ftell(data_bfile_fptr);
    fseek(data_bfile_fptr, 4, SEEK_CUR); // Pular o campo do tamanho
    reg_size += 4;

    set_campo64(-1, data_bfile_fptr); reg_size += 8; // Atribuicao do campo Prox
    set_campo32(j.id, data_bfile_fptr); reg_size += 4; // Atribuicao do campo id
    set_campo32(j.idade, data_bfile_fptr); reg_size += 4; // Atribuicao do campo idade

    // Estou atribuindo o tamanho de cada campo em uma variavel para um futuro tratamento de erro

    int32_t size_aux = 0;
    set_campo_str(j.nome, &size_aux, data_bfile_fptr); reg_size += size_aux; // Atribuicao do campo tamNomeJog e nomeJogador
    set_campo_str(j.nac, &size_aux, data_bfile_fptr); reg_size += size_aux; // Atribuicao do campo tamNacionalidade e nacionalidade
    set_campo_str(j.clube, &size_aux, data_bfile_fptr); reg_size += size_aux; // Atribuicao do campo tamNomeClube e nomeClube
    
    // Voltar no campo tamanhoRegistro e atribuir o valor correto no campo
    fseek(data_bfile_fptr, -(reg_size-1), SEEK_CUR);
    fwrite(&reg_size, 4, 1, data_bfile_fptr);

    // Ir para o fim do registro
    fseek(data_bfile_fptr, reg_size-5, SEEK_CUR);

    return 0;
}

int create_data_file_from_csv(const char *input_filename, const char *output_filename){
    // Abrir o arquivo .csv no modo leitura
    FILE *csv_data_fptr;
    if((csv_data_fptr = fopen(input_filename, "r")) == NULL){
        return -1;
    }

    // Abrir o arquivo de dados binario no modo escrita binaria
    FILE *data_bfile_fptr;
    if((data_bfile_fptr = fopen(output_filename, "wb")) == NULL){
        fclose(csv_data_fptr);
        return -1;
    }

    // Atribuição do status
    set_campoc('0', data_bfile_fptr);

    // Pular cabecalho
    fseek(data_bfile_fptr, HEADER_END_OFFSET, SEEK_SET);

    // Ler a primeira linha do arquivo .csv (colunas)
    char columns[BUFFER_SIZE];
    fgets(columns, BUFFER_SIZE, csv_data_fptr);
    TRIM_NEWLINE(columns);

    // Verifica se as colunas do .csv correspondem ao do projeto
    if(strcmp(columns, COLUMN_NAMES) != 0){
        fclose(csv_data_fptr);
        fclose(data_bfile_fptr);
        return -1; // Posso retornar 1 para indicar um erro lógico, não de saída
    }

    // Contador da quantidade de registros
    int32_t reg_count = 0;

    while(1){
        // Ler linha no csv
        char line_buff[BUFFER_SIZE];
        fgets(line_buff, sizeof(char) * BUFFER_SIZE, csv_data_fptr);

        if(feof(csv_data_fptr)){
            break;
        }

        TRIM_NEWLINE(line_buff);

        // Processa a linha do csv e coloca o jogador no arquivo binario
        JOGADOR j = process_jogador(line_buff);
        add_reg_bfile(j, data_bfile_fptr);
        free_jogador(&j);

        reg_count++;
    }

    // Pega o valor do proxByteOffset (campo do cabecalho do arquivo)
    int64_t prox_byte_offset = ftell(data_bfile_fptr);
    
    // Atribuicao do cabecalho
    fseek(data_bfile_fptr, 0, SEEK_SET);
    initialize_cabecalho('1', -1, prox_byte_offset, reg_count, 0, data_bfile_fptr);

    fclose(csv_data_fptr);
    fclose(data_bfile_fptr);

    return 0;
}