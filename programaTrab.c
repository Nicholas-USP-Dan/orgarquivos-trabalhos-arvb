#include <stdio.h>

int main(){
    // Le um caractere no stdin para verificar qual operacao realizar
    char op;
    scanf("%c", &op);

    // Ramificacao para cada operacao
    switch(op){
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        default:
            fprintf(stderr, "Funcionalidade invalida.\n");
            break;
    }
}