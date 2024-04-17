/**
 * @file filter-data-file.c
 * @brief Implementação da funcionalidade 3 como definido no projeto
 * 
 * O tipo de jogador é utilizado para representar uma busca filtrada (a estrutura contém todos os campos de um registro);
 * Como definido nas especificações do projeto, o id de cada jogador é único, assim, em buscas que filtram o id de um jogador,
 * É possível sair da busca assim que achar-se um registro com o mesmo id que o filtrado
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 1.0
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "data-file.h"

#include "funcoes_fornecidas.h"
#include "cabecalho-utils.h"
#include "campo-utils.h"

/**
 * Instancia de um jogador vazio/nulo
 */
static const JOGADOR jNil = {
    .id = -1,
    .idade = -1,
    .nome = "",
    .nac = "",
    .clube = ""
};

/**
 * Array com todos os nomes dos campos para realizar a filtragem
 */
static const char *CAMPO_LIST[5] = {
    "id",
    "idade",
    "nomeJogador",
    "nacionalidade",
    "nomeClube"
};

/**
 * Funcao para a filtragem de um bit em um bitmask
 */
static int bitmask(unsigned int mask, int bit){
    return (mask & (1 << bit)) >> bit;
}

/**
 * Le uma linha (do stdin) e retorna um jogador com todos os dados da busca definidos; os campos nao procurados ficam vazios;
 * Este jogador representa os campos da filtragem
 * 
 * Alem disso a funcao atribui uma mascara (bitmask) para a filtragem, em que o bit representando um campo e atribuido como 1;
 * 
 * Esta configuracao traz a verificacao mais eficiente dos campos a serem filtrados
 */
static JOGADOR read_query(unsigned int *mask){
    JOGADOR j_query = jNil;
    // Quantidade de campos a serem lidos
    int m;
    scanf("%d", &m);

    (*mask) = 0;

    // Ler o conjunto nomeCampo e valorCampo, atribuir o campo no jogador "filtro" junto com o bit da mascara correspondente
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
    // Abra o arquivo no modo leitura binaria
    FILE *fptr = fopen(input_filename, "rb");
    if(fptr == NULL){
        return -1;
    }

    // Check de status valido
    if(!check_status(fptr)){
        return -1;
    }

    // Loop para fazer n buscas
    for(int i = 0; i < n; i++){
        unsigned int mask;
        // Leitura dos criterios de busca e atribuicao do jogador "filtro"
        JOGADOR j_query = read_query(&mask);

        printf("Busca %d\n\n", i+1);

        int32_t reg_count = 0;
        int32_t filter_count = 0;

        // Ler quantidade de registros existentes para parar de ler objetos removidos
        fseek(fptr, NRO_REGARQ_OFFSET, SEEK_SET);
        int32_t nro_reg;
        fread(&nro_reg, 4, 1, fptr);

        // Pular o cabecalho
        fseek(fptr, HEADER_END_OFFSET, SEEK_SET);

        while(reg_count < nro_reg){ // Le registros ate ler todos os registros validos
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

            // Pular tamanhoRegistro e Prox
            fseek(fptr, 12, SEEK_CUR);

            JOGADOR j = read_jogador_data(fptr);

            // Valor booleano que guarda se o registro passou no filtro
            int filter = (!bitmask(mask, 0) || j_query.id == j.id) && 
                         (!bitmask(mask, 1) || j_query.idade == j.idade) && 
                         (!bitmask(mask, 2) || strcmp(j_query.nome, j.nome) == 0) && 
                         (!bitmask(mask, 3) || strcmp(j_query.nac, j.nac) == 0) && 
                         (!bitmask(mask, 4) || strcmp(j_query.clube, j.clube) == 0);

            if(filter){ // Registro passou pelo filtro
                filter_count++;

                print_jogador(j);
                printf("\n");
            }

            free_jogador(&j);

            reg_count++;

            // Caso o filtro tenha o campo id, quando o jogador com esse id for encontrado, terminar busca
            if(bitmask(mask, 0) && j_query.id == j.id){
                break;
            }
        }

        // Imprimir: "Registro inexistente." caso nenhum registro passar pelo filtro
        if(filter_count <= 0){
            printf("Registro inexistente.\n\n");
        }
    }

    printf("\n");
    fclose(fptr);
    return 0;
}