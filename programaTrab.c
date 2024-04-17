#include <stdio.h>

#include "data-file.h"
#include "funcoes_fornecidas.h"

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
            ret = create_data_file_from_csv(input_filename, output_filename);
            
            if(ret != 0){
                fprintf(stdout, "Falha no processamento do arquivo.\n");
            }
            else{
                binarioNaTela(output_filename);
                printf("\n");
            }
            break;
        case '2':
            scanf("%s", input_filename);
            ret = select_data_file(input_filename);

            if(ret != 0){
                fprintf(stdout, "Falha no processamento do arquivo.\n");
            }
            break;
        case '3':
            scanf("%s", input_filename);
            scanf("%d", &n);

            ret = filter_data_file(n, input_filename);

            if(ret != 0){
                fprintf(stdout, "Falha no processamento do arquivo.\n");
            }
            break; 
        default:
            fprintf(stdout, "Funcionalidade invalida.\n");
            break;
    }
}