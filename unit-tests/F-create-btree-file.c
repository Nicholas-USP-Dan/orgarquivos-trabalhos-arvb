#include <stdio.h>

#include "../src/btree-func.h"

#include "../src/utils/cabecalho-utils.h"
#include "../src/utils/funcoes_fornecidas.h"

int main(){
    char input_filename[200];
    char output_filename[200];
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    FILE *data_fptr = fopen(input_filename, "rb");
    FILE *btree_fptr = fopen(output_filename, "wb+");
    if(!data_fptr || !check_status(data_fptr) || !btree_fptr){
        if(data_fptr) fclose(data_fptr);
        if(btree_fptr) fclose(btree_fptr);
        fprintf(stdout, "Erro no processamento do arquivo\n");
        return -1;
    }

    if(create_btree_file(data_fptr, btree_fptr) == -1){
        perror("Algum erro aconteceu?");
    }

    fclose(data_fptr);
    fclose(btree_fptr);

    binarioNaTela(output_filename);
}