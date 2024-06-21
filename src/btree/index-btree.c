#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "index-btree.h"

#include "index-btree-utils.h"
#include "index-btree-internals.h"
#include "index-btree-cache.h"
#include "index-types.h"

#define NO_PROMO_KEY 0
#define HAS_PROMO_KEY 1

/**
 * @brief Cria uma nova página (que não existe no arquivo) na cache
 *
 * @note Esta função é basicamente uma macro que chama put_page_cache com uma página vazia
 *
 * @param rrn RRN de onde a página se localizaria
 * @param btree Referência ao ponteiro da árvore-B
 * @param btree_fptr Ponteiro para o arquivo da árvore-B
 *
 * @return [BTREE_PAGE*] Retorna o ponteiro da nova página criada na cache
 */
static BTREE_PAGE* create_newpage_cache(const int32_t rrn, BTREE **btree, FILE *btree_fptr){
    BTREE_PAGE newpage;
    newpage.alt = 0;
    newpage.nro_chaves = 0;
    newpage.rrn_filhos[0] = -1;
    for(int i = 0; i < BTREE_ORDER-1; i++){
        newpage.chaves[i] = (INDEX_REG){-1,-1};
        newpage.rrn_filhos[i+1] = -1;
    }

    return put_page_cache(rrn, &newpage, btree, btree_fptr);
}

/**
 * @brief Inicializa a árvore-b
 *
 * @return [BTREE*] Retorna o ponteiro da árvore alocada
 */
BTREE* initialize_btree(){
    BTREE *btree = (BTREE*)malloc(sizeof(BTREE));
    btree->cache = initialize_cache();
    btree->key_count = 0;
    btree->rrn_prox = 0;
    btree->rrn_root = -1;

    return btree;
}

/**
 * @brief Limpa a memória alocada pela árvore-b
 *
 * @param btree Referência ao ponteiro da árvore-b
 */
void clear_btree(BTREE **btree){
    clear_cache(&(*btree)->cache);
    free(*btree);
    *btree = NULL;
}

/**
 * @brief Atribui valores ao cabeçalho
 *
 * @param status Status do arquivo ('0' => instável; '1' => estável)
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página
 */
int set_btree_cabecalho(const char status, BTREE **btree, FILE *btree_fptr){
    fseek(btree_fptr, 0, SEEK_SET);

    unsigned char buffer[BTREE_PAGE_SIZE];
    int off = 0;

    memcpy(&buffer[off], &status, 1); off += 1;
    memcpy(&buffer[off], &(*btree)->rrn_root, 4); off += 4;
    memcpy(&buffer[off], &(*btree)->rrn_prox, 4); off += 4;
    memcpy(&buffer[off], &(*btree)->key_count, 4); off += 4;

    memset(&buffer[off], '$', BTREE_PAGE_SIZE-BTREE_HEADER_SIZE);
    off += BTREE_PAGE_SIZE-BTREE_HEADER_SIZE;
    if(off != 60){
        exit(1);
    }

    fwrite(buffer, 1, BTREE_PAGE_SIZE, btree_fptr);

    return 0;
}

/**
 * @brief Lê o cabeçalho de uma árvore-b de um arquivo
 *
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página ou árvore-b está marcado como instável
 */
int read_btree_cabecalho(BTREE **btree, FILE *btree_fptr){
    fseek(btree_fptr, 0, SEEK_SET);
    unsigned char buffer[BTREE_PAGE_SIZE];

    // Leitura da página inteira para o buffer
    if(fread(buffer, sizeof(unsigned char), BTREE_PAGE_SIZE, btree_fptr) != BTREE_PAGE_SIZE){
        return -1;
    }

    int off = 0;
    if(buffer[off] == '0'){
        // Arquivo marcado como instável
        return -1;
    }
    off++;

    memcpy(&(*btree)->rrn_root, &buffer[off], 4); off += 4;
    memcpy(&(*btree)->rrn_prox, &buffer[off], 4); off += 4;
    memcpy(&(*btree)->key_count, &buffer[off], 4); off += 4;

    return 0;
}

/**
 * @brief Escreve todas as páginas armazenadas na cache no arquivo da árvore-b
 *
 * @details Esta função remove todos os registros da cache, sem limpar a cache em si
 *
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Referência para o arquivo da árvore-b
 *
 * @retval 0 A escrita da página foi feita com sucesso
 * @retval -1 Houve um erro durante a escrita da página
 */
int dump_btree_cache(BTREE **btree, FILE *btree_fptr){
    CACHE_NODE *aux_node = (*btree)->cache->head->next;
    while(aux_node != (*btree)->cache->tail){
        if(update_disk_page(aux_node->rrn, &aux_node->value, btree_fptr) == -1){
            // Houve algum erro guardando os valores da página em memória secundária
            return -1;
        }

        CACHE_NODE *node_temp = aux_node;
        aux_node = aux_node->next;
        free(node_temp);
    }

    (*btree)->cache->head->next = (*btree)->cache->tail;
    (*btree)->cache->tail->prev = (*btree)->cache->head;

    return 0;
}

/**
 * @brief Procura de um certo índice em uma árvore-b
 *
 * @param index Índice a ser procurado
 * @param [out] pos Referência para a variável que guarda a posição do dado (ou o valor onde ele se encaixaria caso não exista)
 * @param page Referência para a página a ser procurada
 *
 * @return [int64_t] Retorna o offset do registro no arquivo de dados que contenha o índice procurado
 * @retval -1 O índice não foi achado na página
 */
static int64_t page_search(const int32_t index, int* pos, const BTREE_PAGE *page){
    int i_aux = 0;
    while(i_aux < page->nro_chaves && index > page->chaves[i_aux].index){
        ++i_aux;
    }

    (*pos) = i_aux;
    if(i_aux < page->nro_chaves && page->chaves[i_aux].index == index){
        return page->chaves[i_aux].offset;
    }

    return -1;
}

/**
 * @brief Componente recursivo da função search_btree
 *
 * @param index Índice a ser procurado
 * @param rrn RRN da página a ser procurada
 * @param btree Referência ao ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @return [int64_t] Retorna o offset do registro no arquivo de dados que contenha o índice procurado
 * @retval -1 O índice não foi achado na página
 */
static int64_t search_btree_rec(const int32_t index, const int32_t rrn, BTREE **btree, FILE *btree_fptr){
    // Caso base
    if(rrn == -1){
        return -1;
    }

    BTREE_PAGE *page;

    if(!(page = get_page(rrn, btree, btree_fptr))){
        // Erro durante a leitura da página
        return -1;
    }

    int pos;
    int64_t res;

    // Achou o elemento na página
    if((res = page_search(index, &pos, page)) != -1){
        return res;
    }

    return search_btree_rec(index, page->rrn_filhos[pos], btree, btree_fptr);
}

int64_t search_btree(const int32_t index, BTREE **btree, FILE *btree_fptr){
    return search_btree_rec(index, (*btree)->rrn_root, btree, btree_fptr);
}

/**
 * @brief Função que verifica se uma página tem espaço para mais registros
 *
 * @param page Referência para a página a ser consultada
 * @retval 0 Há mais espaço na página
 * @retval 1 Não há mais espaço na página
 */
static inline int has_space_page(const BTREE_PAGE *page){
    return page->nro_chaves < BTREE_ORDER-1;
}

/**
 * @brief Insere um registro em uma página não cheia
 *
 * @note A função não verifica se a página ainda tem espaço ou não, então é necessário verificar se a página não está cheia
 * antes de chamar a função
 *
 * @param reg Registro a  ser inserido
 * @param pos Posição onde o registro deve ficar
 * @param page_dest Referência para a página a ter um registro inserido
 * @param rrn_child RRN do filho a direita do registro (origem do registro na chamada recursiva)
 */
static void insert_page_nonfull(const INDEX_REG reg, const int pos, BTREE_PAGE *page_dest, const int32_t rrn_child){
    for(int i = page_dest->nro_chaves; i > pos; i--){
        page_dest->chaves[i] = page_dest->chaves[i-1];
        page_dest->rrn_filhos[i+1] = page_dest->rrn_filhos[i];
    }

    page_dest->chaves[pos] = reg;
    page_dest->rrn_filhos[pos+1] = rrn_child;
    page_dest->nro_chaves++;
}

/**
 * @brief Divide uma página com overflow em duas, enviando um elemento para ser promovido no nível superior
 *
 * @param reg Registro que está causando o overflow
 * @param rrn_child RRN da página que o registro vem
 * @param pos Posição onde o registro deveria ficar (se não causasse overflow)
 * @param rrn_root RRN da página a ser dividida
 * @param [out] promo_reg Referência para o registro a ser promovido
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 * 
 * @return [int] Retorna o RRN da página criada
 * @retval -1 Houve algum erro durante a operação
 */
static int32_t split_page(const INDEX_REG reg, const int rrn_child, const int pos, const int32_t rrn_root, INDEX_REG *promo_reg, BTREE **btree, FILE *btree_fptr){
    BTREE_PAGE *root_page;
    if(!(root_page = get_page(rrn_root, btree, btree_fptr))){
        return -1;
    }

    int32_t ret = (*btree)->rrn_prox;
    BTREE_PAGE *new_page;
    if(!(new_page = create_newpage_cache((*btree)->rrn_prox++, btree, btree_fptr))){
        return -1;
    }
    new_page->alt = root_page->alt;

    INDEX_REG temp_keys[BTREE_ORDER];
    int32_t temp_rrn_filhos[BTREE_ORDER+1];

    // NOTE: a forma com memcpy requer uma cópia inteira, depois uma inserção, ou 2 memcpy's fragmentadas,
    // Assim essa forma é tanto mais performática quanto mais simples
    int reg_i = BTREE_ORDER-1;
    if(reg_i == pos){
        temp_keys[reg_i--] = reg;
    }
    for(int i = BTREE_ORDER-2; i >= 0; i--){
        temp_keys[reg_i--] = root_page->chaves[i];
        if(i == pos){
            temp_keys[reg_i--] = reg;
        }
    }

    int child_i = BTREE_ORDER;
    if(child_i == pos+1){
        temp_rrn_filhos[child_i--] = rrn_child;
    }
    for(int i = BTREE_ORDER-1; i >= 0; i--){
        temp_rrn_filhos[child_i--] = root_page->rrn_filhos[i];
        if(i == pos+1){
            temp_rrn_filhos[child_i--] = rrn_child;
        }
    }

    int mid = (BTREE_ORDER-1)/2;
    (*promo_reg) = temp_keys[mid];

    new_page->nro_chaves = BTREE_ORDER - (mid+1);
    memcpy(new_page->chaves, &temp_keys[mid+1], sizeof(INDEX_REG) * new_page->nro_chaves);
    memcpy(new_page->rrn_filhos, &temp_rrn_filhos[mid+1], sizeof(int32_t) * (new_page->nro_chaves+1));

    for(int i = BTREE_ORDER - 2; i >= 0; i--){
        if(i >= mid){
            root_page->chaves[i] = (INDEX_REG){-1,-1};
            root_page->rrn_filhos[i+1] = -1;
        }
        // else{
        //     root_page->chaves[i] = temp_keys[i];
        //     root_page->rrn_filhos[i+1] = temp_rrn_filhos[i+1];
        // }
    }

    // root_page->rrn_filhos[0] = temp_rrn_filhos[0];

    memcpy(root_page->chaves, temp_keys, sizeof(INDEX_REG) * mid);
    memcpy(root_page->rrn_filhos, temp_rrn_filhos, sizeof(int32_t) * (mid+1));

    root_page->nro_chaves = mid;

    return ret;
}

static int insert_btree_rec(const INDEX_REG reg, const int32_t rrn_root, INDEX_REG *promo_reg, int32_t *rrn_promo, int32_t *alt, BTREE **btree, FILE *btree_fptr){
    if(rrn_root == -1){
        *promo_reg = reg;
        *rrn_promo = -1;
        (*btree)->key_count++;
        *alt = -1;
        return HAS_PROMO_KEY;
    }

    BTREE_PAGE *page;
    if(!(page = get_page(rrn_root, btree, btree_fptr))){
        fprintf(stderr, "Problema ao ler a página no arquivo da árvore-b\n");
        return -1;
    }

    int pos;
    // Faço verificação se já existe?
    page_search(reg.index, &pos, page);

    INDEX_REG reg_promo_buff;
    int32_t rrn_promo_buff, alt_buff;
    int ret = insert_btree_rec(reg, page->rrn_filhos[pos], &reg_promo_buff, &rrn_promo_buff, &alt_buff, btree, btree_fptr);

    *alt = alt_buff+1;

    if(ret != HAS_PROMO_KEY){
        return ret;
    }

    page = get_page(rrn_root, btree, btree_fptr);

    // Insere em uma página não cheia
    if(has_space_page(page)){
        insert_page_nonfull(reg_promo_buff, pos, page, rrn_promo_buff);
        return NO_PROMO_KEY;
    }

    // Split

    *rrn_promo = split_page(reg_promo_buff, rrn_promo_buff, pos, rrn_root, promo_reg, btree, btree_fptr);
    return HAS_PROMO_KEY;
}

int insert_btree(const INDEX_REG reg, BTREE **btree, FILE *btree_fptr){
    INDEX_REG reg_promo_buff;
    int32_t rrn_promo_buff;
    int32_t alt_buff;
    int ret = insert_btree_rec(reg, (*btree)->rrn_root, &reg_promo_buff, &rrn_promo_buff, &alt_buff, btree, btree_fptr);

    if(ret == HAS_PROMO_KEY){
        // Criar página para o topo
        BTREE_PAGE *new_page;
        if(!(new_page = create_newpage_cache((*btree)->rrn_prox, btree, btree_fptr))){
            return -1;
        }

        insert_page_nonfull(reg_promo_buff, 0, new_page, rrn_promo_buff);

        new_page->alt = alt_buff+1;
        new_page->rrn_filhos[0] = (*btree)->rrn_root;
        (*btree)->rrn_root = (*btree)->rrn_prox++;
        // (*btree)->rrn_prox++;
    }

    return ret;
}