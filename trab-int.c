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

#include "src/data-file.h"
#include "src/utils/funcoes_fornecidas.h"

int main(){
    // Le um caractere no stdin para verificar qual operacao realizar
    char op;
    scanf("%c", &op);

    // Ramificacao para cada operacao
    switch(op){
        char input_filename[200];
        char output_filename[200];
        int n;
        int ret;
        case '1':
            scanf("%s", input_filename);
            scanf("%s", output_filename);
            ret = create_data_file(input_filename, output_filename);
            
            if(ret != 0){
                fprintf(stdout, "Falha no processamento do arquivo.\n");
            }
            else{
                binarioNaTela(output_filename);
                //printf("\n");
            }
            break;
        case '2':
            scanf("%s", input_filename);
            ret = select_data(input_filename);

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
            break;
    }
}