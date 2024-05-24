#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/utils/cabecalho-utils.h"
#include "../src/utils/removed-list.h"

int main(){
    FILE *data_fptr = NULL;
    FILE *rem_fptr = NULL;
    if(!(data_fptr = fopen("test-cases/trab-1/Saída/binario5.bin", "rb")) || !check_status(data_fptr) ||
    !(rem_fptr = fopen("teste.bin", "wb"))){
        if(data_fptr) fclose(data_fptr);
        if(rem_fptr) fclose(rem_fptr);
        perror(NULL);
        return EXIT_FAILURE;
    }

    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

    write_rem_list(rem_fptr, &rem_list);    

    clear_rem_list(&rem_list);
}