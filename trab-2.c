/**
 * @file trab-2.c
 * @brief Arquivo do programa principal do projeto.
 * 
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 * 
 * @version 2.0
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "src/btree/index-btree.h"
#include "src/utils/campo-utils.h"
#include "src/utils/data-utils.h"
#include "src/utils/cabecalho-utils.h"
#include "src/utils/funcoes_fornecidas.h"

#include "src/data-file.h"
#include "src/utils/removed-list.h"
#include "src/btree-func.h"

static int func7(){
    char input_filename[200];
    char output_filename[200];
    int ret;

    FILE *btree_fptr = NULL;
    FILE *data_fptr = NULL;

    // Input do usuário
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    // Abre os arquivos, se algum erro for detectado, retorna
    if(!(data_fptr = fopen(input_filename, "r")) || !check_status(data_fptr) || 
    !(btree_fptr = fopen(output_filename, "wb+"))){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        
        if(data_fptr) fclose(data_fptr);
        if(btree_fptr) fclose(btree_fptr);

        return -1;
    }

    // ret = create_data_file(data_fptr, btree_fptr);
    ret = create_btree_file(data_fptr, btree_fptr);
    
    // Fecha os arquivos
    fclose(data_fptr);
    fclose(btree_fptr);

    // Imprime o binarioNaTela do arquvo criado caso a funcionalidade tenha terminado sem erro ou uma mensagem
    // de erro caso contrário 
    ret == 0 ? binarioNaTela(output_filename) : 
                fprintf(stdout, "Falha no processamento do arquivo.\n");

    return ret;
}

int static inline func8(){
    char input_filename[200];
    char output_filename[200];
    int n;

    FILE *data_fptr = NULL;
    FILE *btree_fptr = NULL;

    // Input do ususário
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    scanf("%d", &n);

    // Abre os arquivos, se algum erro for encontrado, retorna
    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr) ||
    !(btree_fptr = fopen(output_filename, "rb")) || !check_status(btree_fptr)){

        fprintf(stdout, "Falha no processamento do arquivo.\n");

        if(data_fptr) fclose(data_fptr);
        if(btree_fptr) fclose(btree_fptr);

        return -1;
    }

    BTREE *btree = initialize_btree();
    if(read_btree_cabecalho(&btree, btree_fptr) == -1){
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        fclose(data_fptr);
        fclose(btree_fptr);

        return -1;
    }

    // Faz n buscas
    for(int i = 0; i < n; i++){
        // Lê o id escrito pelo usuário
        char buffer[200];
        scanf("%s", buffer);

        // if(strcmp("id", buffer) != 0){
        //     break;
        // }

        int32_t id;
        scanf("%" PRId32, &id);

        printf("BUSCA %d\n\n", i+1);

        int64_t offset = search_btree(id, &btree, btree_fptr);

        if(offset == -1){
            printf("Registro inexistente.\n\n");
            continue;
        }

        fseek(data_fptr, offset + 13, SEEK_SET);
        JOGADOR j = read_jogador_data(data_fptr);

        print_jogador(j);
        printf("\n");
    }

    clear_btree(&btree);

    // Fecha o arquivo
    fclose(data_fptr);
    fclose(btree_fptr);

    return 0;
}

int static inline func9(){
    char input_filename[200];
    char output_filename[200];
    int n;
    // int ret;

    FILE *data_fptr = NULL;
    FILE *btree_fptr = NULL;

    // Input do usuário
    scanf("%s", input_filename);
    scanf("%s", output_filename);

    scanf("%d", &n);

    // Abre os arquivos, se algum erro for encontrado, retorna
    if(!(data_fptr = fopen(input_filename, "rb")) || !check_status(data_fptr) ||
    !(btree_fptr = fopen(output_filename, "rb")) || !check_status(btree_fptr)){

        fprintf(stdout, "Falha no processamento do arquivo.\n");

        if(data_fptr) fclose(data_fptr);
        if(btree_fptr) fclose(btree_fptr);

        return -1;
    }

    BTREE *btree = initialize_btree();
    if(read_btree_cabecalho(&btree, btree_fptr) == -1){
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        fclose(data_fptr);
        fclose(btree_fptr);

        return -1;
    }

    // Faz n buscas
    for(int i = 0; i < n; i++){
        JOGADOR j_query = read_query();

        printf("Busca %d\n\n", i+1);
        
        select_btree_data(data_fptr, &btree, btree_fptr, j_query);
    }
    clear_btree(&btree);

    // Fecha o arquivo
    fclose(data_fptr);
    fclose(btree_fptr);

    // ret == 0 ? binarioNaTela(output_filename) : 
    //             fprintf(stdout, "Falha no processamento do arquivo.\n");

    return 0;
}

int static inline func10(){
    char input_filename[200];
    char btree_filename[200];

    int n;
    int ret;

    FILE *data_fptr = NULL;
    FILE *btree_fptr = NULL;

    // Leitura de input do usuário
    scanf("%s", input_filename);
    scanf("%s", btree_filename);

    // Abre os arquivos, se algum erro for detectado, retorna
    if(!(data_fptr = fopen(input_filename, "r+b")) || !check_status(data_fptr) || 
    !(btree_fptr = fopen(btree_filename, "r+b")) || !check_status(btree_fptr)){
        fprintf(stdout, "Falha no processamento do arquivo.\n");
        
        if(data_fptr) fclose(data_fptr);
        if(btree_fptr) fclose(btree_fptr);

        return -1;
    }

    // Leitura da quantidade de remoções a serem realizadas
    scanf("%d", &n);

    // Setta os status do arquivo de dados e de índice como '0' (instável)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    set_campoc('0', data_fptr);

    // Inicializa a árvore-b, lendo os valores do cabeçalho
    BTREE *btree = initialize_btree();
    if (read_btree_cabecalho(&btree, btree_fptr) == -1) {
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        fclose(data_fptr);
        fclose(btree_fptr);

        return -1;
    }

    /* Atribui o status da árvore-b como '0'; 
    Os outros dados do cabecalho da arvore-b Não muda, então seria possível atribuir 
    somente o campo de status, mas como a atribuição é feita por páginas, não há 
    diferença na quantidade de seeks realiazdos */
    set_btree_cabecalho('0', &btree, btree_fptr);

    // Array com a lista de registros removidos
    REM_LIST *rem_list = load_rem_list(data_fptr, BEST_FIT);

    // Variável que mantém a quantidade de elementos removidos na operação
    int quant_ins = 0;

    for (int i = 0; i < n; i++){
        // JOGADOR j_query = read_query();
        JOGADOR j_query = read_new_jogador();
        
        ret = insert_data_btree(data_fptr, j_query, &quant_ins, &rem_list, &btree, btree_fptr);
        if(ret != 0) fprintf(stdout, "Falha no processamento do arquivo.\n");

        free_jogador(&j_query);
    }

    // Escrever as estruturas em memória secundária
    dump_btree_cache(&btree, btree_fptr);
    write_rem_list(&rem_list, data_fptr);

    // Atualiza os campos com a quantidade de registros no cabeçalho do arquivo de dados
    update_nro_reg(quant_ins, data_fptr);

    // Setta os status do arquivo de dados como '1' (estável)
    fseek(data_fptr, STATUS_OFFSET, SEEK_SET);
    set_campoc('1', data_fptr);

    // Setta o status da árvore-b como '1' e atualiza os valores do cabeçalho da árvore-b
    set_btree_cabecalho('1', &btree, btree_fptr);

    // Limpeza de memória das estruturas utilizadas (lista de índices e de removidos)
    clear_btree(&btree);
    clear_rem_list(&rem_list);

    // Fecha os arquivos
    fclose(data_fptr);
    fclose(btree_fptr);

    // Imprime os bináriosNaTela dos arquivos
    binarioNaTela(input_filename);
    binarioNaTela(btree_filename);

    return 0;
}

int main(){
    // Le uma string no stdin para verificar qual operacao realizar

    char op[50];
    scanf("%49s", op);

    if (strcmp(op, "7") == 0) {
        func7();
    }
    else if (strcmp(op, "8") == 0) {
        func8();
    }
    else if (strcmp(op, "9") == 0) {
        func9();
    }
    else if (strcmp(op, "10") == 0) {
        func10();
    }
    else {
        fprintf(stdout, "Funcionalidade invalida.\n");
    }

    // return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE; -- Não pode retornar erro no programa
    return 0;
}