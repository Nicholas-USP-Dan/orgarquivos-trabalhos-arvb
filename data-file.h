#include <stdint.h>

#define COLUMN_NAMES "id,idade,nomeJogador,nacionalidade,nomeClube"

/**
 * Tipo da estrutura representando os dados de um jogador;
 */
typedef struct _jogador {
    int32_t id;
    int32_t idade;
    char *nome;
    char *nac;
    char *clube;
} JOGADOR;

/**
 * Limpa a memria alocada por um jogador (strings) 
 */
void free_jogador(JOGADOR *j);

/**
 * Funcao que imprime um jogador de acordo com os requisitos do projeto 
 */
void print_jogador(JOGADOR j);

/**
 * Le um registro em fptr e atribui os valores na variavel JOGADOR
 */
JOGADOR read_jogador_data(FILE *fptr);

/**
 * Funcao da funcionalidade 1 - Criar um arquivo binario de dados a partir de um arquivo de dados .csv
 * 
 * RETORNA: Retorna 0 qundo o arquivo é criado com sucesso, caso nao retorna -1
 */
int create_data_file_from_csv(const char *input_filename, const char *output_filename);

/**
 * Funcao da funcionalidade 2 - Imprime todos os registros validos (nao removidos) de um arquivo binario de dados
 */
int select_data_file(const char *input_filename);

/**
 * Funcao da funcionalidade 3 - Filtra os registros validos (nao removidos) de um arquivo binario de dados
 * 
 * Se realiza n pesquisar, cada uma sendo rotulada como Busca x
 */
int filter_data_file(const int n, const char *input_filename);