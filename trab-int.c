/**
 * @file trab-int.c
 * @brief Arquivo do programa principal do projeto.
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "src/data-file.h"
#include "src/utils/funcoes_fornecidas.h"

int main(){
    // Le um caractere no stdin para verificar qual operacao realizar
    char op;
    scanf("%c", &op);

    char input_filename[200];
    char output_filename[200];
    int n;
    int ret;
    // int update_delete;
    // int update_index;

    FILE *data_fptr = NULL;
    FILE *csv_fptr = NULL;
    // FILE *index_fptr = NULL;

    // Ramificacao para cada operacao
    switch(op){
        case '1':
            scanf("%s", input_filename);
            scanf("%s", output_filename);

            // Abertura dos arquivos
            if(!(csv_fptr = fopen(input_filename, "r")) || !(data_fptr = fopen(output_filename, "wb"))){
                if(csv_fptr) fclose(csv_fptr);
                if(data_fptr) fclose(data_fptr);
                fprintf(stdout, "Falha no processamento do arquivo.\n");

                break;
            }

            ret = create_data_file(csv_fptr, data_fptr);

            fclose(csv_fptr);
            fclose(data_fptr);

            if(ret != 0){
                fprintf(stdout, "Falha no processamento do arquivo.\n");
            }
            else{
                binarioNaTela(output_filename);
            }

            break;
        case '2':
            scanf("%s", input_filename);
            FILE *data_fptr = fopen(input_filename, "rb");
            if(!data_fptr){
                ret = -1;
                break;
            }

            ret = select_data(data_fptr);

            if(ret != 0){
                fprintf(stdout, "Falha no processamento do arquivo.\n");
            }
            break;
        case '3':
            scanf("%s", input_filename);
            scanf("%d", &n);

            for(int i = 0; i < n; i++){
                printf("Busca %d\n\n", i+1);

                ret = filter_data(input_filename);
                if(ret != 0){
                    fprintf(stdout, "Falha no processamento do arquivo.\n");
                    break;
                }
            }
            break; 
        default:
            fprintf(stdout, "Funcionalidade invalida.\n");
            ret = -1;
            break;
    }

    return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}