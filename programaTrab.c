#include <stdio.h>

int main(){
    // Le um caractere no stdin para verificar qual operacao realizar
    char op;
    scanf("%c", &op);

    // Ramificacao para cada operacao
    switch(op){
        char *input_filename;
        char *output_filename;
        int n;
        case '1':
            scanf("%s", input_filename);
            scanf("%s", output_filename);
            break;
        case '2':
            scanf("%s", input_filename);
            break;
        case '3':
            scanf("%s", input_filename);
            scanf("%d", &n);
            break;
        default:
            fprintf(stderr, "Funcionalidade invalida.\n");
            break;
    }
}