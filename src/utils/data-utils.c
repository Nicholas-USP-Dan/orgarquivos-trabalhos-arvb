/**
 * @file data-file-utils.c
 * @brief Source file de algumas utilidades gerais do arquivo binário de dados
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

#include "data-utils.h"

#include "campo-utils.h"
#include "funcoes_fornecidas.h"

/**
 * @brief Instância de um jogador vazio/nulo, representa uma busca "vazia"
 */
const JOGADOR jNil = {
    .id = -1,
    .idade = -1,
    .nome = "",
    .nac = "",
    .clube = ""
};

/**
 * @brief Array com todos os nomes dde campos válidos para realizar a filtragem
 */
const char *CAMPO_LIST[5] = {
    "id",
    "idade",
    "nomeJogador",
    "nacionalidade",
    "nomeClube"
};

void free_jogador(JOGADOR *j){
    free(j->nome);
    j->nome = NULL;
    free(j->nac);
    j->nac = NULL;
    free(j->clube);
    j->clube = NULL;
}

void print_jogador(const JOGADOR j){
    // printf("ID do Jogador: %" PRId32 "\n", j.id);

    // printf("Idade do Jogador: %" PRId32 "\n", j.idade);

    char *nome = strcmp(j.nome, "") != 0 ? j.nome : "SEM DADO";
    printf("Nome do Jogador: %s\n", nome);

    char *nac = strcmp(j.nac, "") != 0 ? j.nac : "SEM DADO";
    printf("Nacionalidade do Jogador: %s\n", nac);

    char *clube = strcmp(j.clube, "") != 0 ? j.clube : "SEM DADO";
    printf("Clube do Jogador: %s\n", clube);
}

JOGADOR read_jogador_data(FILE *fptr){
    JOGADOR j;

    j.id = get_campo32(fptr);
    j.idade = get_campo32(fptr);
    j.nome = get_campo_str(fptr);
    j.nac = get_campo_str(fptr);
    j.clube = get_campo_str(fptr);

    return j;
}

JOGADOR read_query(){
    JOGADOR j_query = jNil;

    // Quantidade de campos a serem lidos
    int m;
    scanf("%d", &m);

    j_query.nome = calloc(sizeof(char), BUFFER_SIZE);
    j_query.nac = calloc(sizeof(char), BUFFER_SIZE);
    j_query.clube = calloc(sizeof(char), BUFFER_SIZE);

    // Ler o conjunto nomeCampo e valorCampo, atribuir o campo no jogador "filtro" junto com o bit da mascara correspondente
    for(int i = 0; i < m; i++){
        char campo[BUFFER_SIZE];
        scanf("%s", campo);

        if(strcmp(campo, CAMPO_LIST[0]) == 0){
            scanf("%" PRId32, &j_query.id);
        }
        else if(strcmp(campo, CAMPO_LIST[1]) == 0){
            scanf("%" PRId32, &j_query.idade);
        }
        else if(strcmp(campo, CAMPO_LIST[2]) == 0){
            scan_quote_string(j_query.nome);
        }
        else if(strcmp(campo, CAMPO_LIST[3]) == 0){
            scan_quote_string(j_query.nac);
            printf("passou pela nacionalidade; %s\n", j_query.nac);
        }
        else if(strcmp(campo, CAMPO_LIST[4]) == 0){
            scan_quote_string(j_query.clube);
        }
    }

    return j_query;
}

static unsigned int get_mask(const JOGADOR where){
    unsigned int mask = 0;
    if(where.id != jNil.id){
        mask |= IDMASK;
    }
    if(where.idade != jNil.idade){
        mask |= IDADEMASK;
    }
    if(strcmp(where.nome, jNil.nome) != 0){
        mask |= NOMEMASK;
    }
    if(strcmp(where.nac, jNil.nac) != 0){
        mask |= NACMASK;
    }
    if(strcmp(where.clube, jNil.clube) != 0){
        mask |= CLUBEMASK;
    }

    return mask;
}

int pass_where(const JOGADOR j, const JOGADOR where){
    unsigned int mask = get_mask(where);
    return  (!(mask & IDMASK) || where.id == j.id) && 
            (!(mask & IDADEMASK) || where.idade == j.idade) && 
            (!(mask & NOMEMASK) || strcmp(where.nome, j.nome) == 0) && 
            (!(mask & NACMASK) || strcmp(where.nac, j.nac) == 0) && 
            (!(mask & CLUBEMASK) || strcmp(where.clube, j.clube) == 0);
}

/**
 * @brief Adiciona um registro em um arquivo binário de dados
 * 
 * @param j Dados do registro a ser inserido.
 * @param data_fptr Ponteiro para o arquivo binário de dados
 * 
 * @retval -1 Houve um erro durante a adição do registro no arquivo binário.
 * @retval 0 Registro adicionado no arquivo binário com sucesso.
 */
static int append_reg(const JOGADOR j, FILE *data_fptr){
    int32_t reg_size = 0; // Variável que guarda o tamanho total do registro

    set_campoc('0', data_fptr); reg_size += 1; // Atribuição do campo removido como '0'

    // Pular o campo tamanhoReg
    fseek(data_fptr, 4, SEEK_CUR); reg_size += 4;

    set_campo64(-1, data_fptr); reg_size += 8; // Atribuicao do campo Prox

    int32_t size_aux = 0; // Variável auxiliar para guardar o espaço ocupado pelos campos de string

    set_campo32(j.id, data_fptr); reg_size += 4; // Atribuicao do campo id
    set_campo32(j.idade, data_fptr); reg_size += 4; // Atribuicao do campo idade
    set_campo_str(j.nome, &size_aux, data_fptr); reg_size += size_aux; // Atribuicao do campo tamNomeJog e nomeJogador
    set_campo_str(j.nac, &size_aux, data_fptr); reg_size += size_aux; // Atribuicao do campo tamNacionalidade e nacionalidade
    set_campo_str(j.clube, &size_aux, data_fptr); reg_size += size_aux; // Atribuicao do campo tamNomeClube e nomeClube
    
    // Voltar no campo tamanhoRegistro e atribuir o valor correto no campo
    fseek(data_fptr, -(reg_size-1), SEEK_CUR);
    fwrite(&reg_size, 4, 1, data_fptr);

    // Voltar para o fim do registro
    fseek(data_fptr, reg_size-5, SEEK_CUR);

    return 0;
}

// int delete_reg(FILE *data_fptr, REM_LIST **rem_list, int offset){
//     fseek(data_fptr, offset, SEEK_SET);
//     int rem = 1;
//     fwrite(&rem, 1, 1, data_fptr);

//     fseek(data_fptr, offset, SEEK_SET);
//     fread(&rem, 1, 1, fp);

//     if (rem != '1'){
//         return EXIT_FAILURE;
//     }

//     add_rem_list(FILE *data_fptr, REM_LIST **rem_list, int offset);


//     return rem == '1';
// }