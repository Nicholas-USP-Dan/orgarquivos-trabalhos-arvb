#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "../src/utils/cabecalho-utils.h"
#include "../src/utils/removed-list.h"

int main() {
    char input_filename[200];
    scanf("%s", input_filename);
    FILE *data_fptr = fopen(input_filename, "rb");
    if(!data_fptr || !check_status(data_fptr)){
        perror(NULL);
        return 1;
    }

    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

    for(int i = 0; i < get_len_dynarr(&(*rem_list).arr); i++){
        REM_EL* ptr = (REM_EL*)get_dynarr(i, &(*rem_list).arr);
        if(ptr) {
            printf("tam: %" PRId32 "; offset: 0x%" PRIX64 "\n", ptr->tam, ptr->offset);
        }
        else {
            printf("Nulo\n");
        }
    }

    clear_rem_list(&rem_list);
}