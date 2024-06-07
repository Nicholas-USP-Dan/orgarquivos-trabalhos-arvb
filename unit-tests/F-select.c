#include <stdio.h>
#include <assert.h>

#include "../src/data-file.h"
#include "../src/utils/data-utils.h"
#include "../src/utils/cabecalho-utils.h"

int main(){
    char input_filename[200];
    scanf("%s", input_filename);
    FILE *data_fptr = fopen(input_filename, "rb");
    if(!data_fptr || !check_status(data_fptr)){
        perror(NULL);
        return 1;
    }

    int ret;

    JOGADOR j_query = read_query();

    printf("===== j_query =====\n");

    print_jogador(j_query);

    printf("===================\n");

    ret = select_data(data_fptr, j_query);

    if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

    fseek(data_fptr, 0, SEEK_SET);

    free_jogador(&j_query);

    fclose(data_fptr);
}