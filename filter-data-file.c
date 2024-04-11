#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "funcoes_fornecidas.h"
#include "cabecalho-utils.h"
#include "campo-utils.h"

static const JOGADOR jNil = {
    .id = -1,
    .idade = -1,
    .nome = "",
    .nac = "",
    .clube = ""
};

static const char *CAMPO_LIST[5] = {
    "id",
    "idade",
    "nomeJogador",
    "nacionalidade",
    "nomeClube"
};

static int bitmask(unsigned int mask, int bit){
    return (mask & (1 << bit)) >> bit;
}

/**
 * Le uma linha (do stdin) e retorna um jogador com todos os dados da busca definidos; os campos nao procurados ficam vazios
 */
static JOGADOR read_query(unsigned int *mask){
    JOGADOR j_query = jNil;
    int m;
    scanf("%d", &m);

    for(int i = 0; i < m; i++){
        char campo[BUFFER_SIZE];
        scanf("%s", campo);

        if(strcmp(campo, CAMPO_LIST[0]) == 0){
            scanf("%" PRId32, &j_query.id);
            (*mask) |= 0x01;
        }
        else if(strcmp(campo, CAMPO_LIST[1]) == 0){
            scanf("%" PRId32, &j_query.idade);
            (*mask) |= 0x02;
        }
        else if(strcmp(campo, CAMPO_LIST[2]) == 0){
            j_query.nome = malloc(sizeof(char) * BUFFER_SIZE);
            scan_quote_string(j_query.nome);

            (*mask) |= 0x04;
        }
        else if(strcmp(campo, CAMPO_LIST[3]) == 0){
            j_query.nac = malloc(sizeof(char) * BUFFER_SIZE);
            scan_quote_string(j_query.nac);

            (*mask) |= 0x08;
        }
        else if(strcmp(campo, CAMPO_LIST[4]) == 0){
            j_query.clube = malloc(sizeof(char) * BUFFER_SIZE);
            scan_quote_string(j_query.clube);
            
            (*mask) |= 0x10;
        }
        else{
            return jNil;
        }
    }

    return j_query;
}

int filter_data_file(const int n, const char *input_filename){
    FILE *fptr = fopen(input_filename, "rb");
    if(fptr == NULL){
        return -1;
    }

    // Check de status valido
    if(!check_status(fptr)){
        return -1;
    }

    for(int i = 0; i < n; i++){
        unsigned int mask = 0;
        JOGADOR j_query = read_query(&mask);

        printf("Busca %d\n\n", i+1);

        int32_t filter_count = 0;

        fseek(fptr, HEADER_END_OFFSET, SEEK_SET);

        while(1){
            JOGADOR j;

            unsigned char rem = get_campoc(fptr);

            if(feof(fptr)){
                break;
            }

            // Registro esta removido, mover para o proximo
            if(rem == '1'){
                int32_t reg_size;
                fread(&reg_size, 4, 1, fptr);
                fseek(fptr, reg_size-5, SEEK_CUR);
                continue;
            }

            // Pular tamanhoRegistro Prox
            fseek(fptr, 12, SEEK_CUR);

            j = read_jogador_data(fptr);

            int filter = (!bitmask(mask, 0) || j_query.id == j.id) && 
                         (!bitmask(mask, 1) || j_query.idade == j.idade) && 
                         (!bitmask(mask, 2) || strcmp(j_query.nome, j.nome) == 0) && 
                         (!bitmask(mask, 3) || strcmp(j_query.nac, j.nac) == 0) && 
                         (!bitmask(mask, 4) || strcmp(j_query.clube, j.clube) == 0);

            if(filter){
                filter_count++;

                print_jogador(j);
                printf("\n");
            }

            free_jogador(&j);
        }

        if(filter_count <= 0){
            printf("Registro inexistente.\n\n");
        }
    }

    printf("\n");
    fclose(fptr);
    return 0;
}