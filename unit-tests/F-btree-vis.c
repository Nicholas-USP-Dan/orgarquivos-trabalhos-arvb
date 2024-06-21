#include <stdio.h>
// #include <stdlib.h>
#include <inttypes.h>

#include "../src/btree/index-btree.h"
#include "../src/btree/index-btree-debug.h"

int main(){
    BTREE *btree = initialize_btree();

    char input_filename[200];
    char output_filename[200];
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    FILE *btree_fptr = fopen(input_filename, "rb+");
    FILE *btree_output = fopen(output_filename, "w");
    if(!btree_fptr || !btree_output){
        if(btree_fptr) fclose(btree_fptr);
        if(btree_output) fclose(btree_output);
    }

    if(read_btree_cabecalho(&btree, btree_fptr) == -1){
        clear_btree(&btree);

        fclose(btree_output);
        fclose(btree_fptr);
        return 1;
    }

    gen_visual(&btree, btree_fptr, btree_output);

    clear_btree(&btree);

    fclose(btree_output);
    fclose(btree_fptr);
}