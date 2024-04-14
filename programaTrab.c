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
        case '1':
            scanf("%s", input_filename);
            scanf("%s", output_filename);
            int ret = create_data_file_from_csv(input_filename, output_filename);
            
            if(ret != 0){
                fprintf(stderr, "Falha no processamento do arquivo.\n");
            }
            else{
                binarioNaTela(output_filename);
                printf("\n");
            }
            break;
        case '2':
            scanf("%s", input_filename);
            break;
        case '3':
            scanf("%s", input_filename);
            scanf("%d", &n);

            filter_data_file(n, input_filename);
            break;
        default:
            fprintf(stderr, "Funcionalidade invalida.\n");
            break;
    }
}