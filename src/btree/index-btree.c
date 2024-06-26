/**
 * @file index-btree.c
 * @brief Source file para as funcionalidades da árvore-b
 *
 * @authors Nicholas Eiti Dan; N°USP: 14600749
 * @authors Laura Neri Thomaz da Silva; N°USP: 13673221
 *
 * @version 3.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "index-btree.h"

#include "index-btree-internals.h"
#include "index-btree-cache.h"
#include "index-types.h"

#define NO_PROMO_KEY 0
#define HAS_PROMO_KEY 1

BTREE* initialize_btree(){
    BTREE *btree = (BTREE*)malloc(sizeof(BTREE));
    btree->cache = initialize_cache();
    btree->key_count = 0;
    btree->rrn_prox = 0;
    btree->rrn_root = -1;

    return btree;
}

void clear_btree(BTREE **btree){
    clear_cache(&(*btree)->cache);
    free(*btree);
    *btree = NULL;
}

int set_btree_cabecalho(const char status, BTREE **btree, FILE *btree_fptr){
    // Posicionamento do ponteiro do arquivo no início do arquivo
    fseek(btree_fptr, 0, SEEK_SET);

    // Escrita dos valores do cabeçalho da árvore-b para o buffer
    unsigned char buffer[BTREE_PAGE_SIZE];
    int off = 0;

    memcpy(&buffer[off], &status, 1); off += 1;
    memcpy(&buffer[off], &(*btree)->rrn_root, 4); off += 4;
    memcpy(&buffer[off], &(*btree)->rrn_prox, 4); off += 4;
    memcpy(&buffer[off], &(*btree)->key_count, 4); off += 4;

    memset(&buffer[off], '$', BTREE_PAGE_SIZE-BTREE_HEADER_SIZE);
    off += BTREE_PAGE_SIZE-BTREE_HEADER_SIZE;

    if(off != 60){
        // Erro caso todo o buffer não tenha sido preenchido
        return -1;
    }

    // Escrita do buffer no arquivo da árvore-b
    fwrite(buffer, 1, BTREE_PAGE_SIZE, btree_fptr);

    return 0;
}

int read_btree_cabecalho(BTREE **btree, FILE *btree_fptr){
    // Posicionamento do ponteiro do arquivo no início do arquivo
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

    // Atribuição dos valores da estrutura da árvore-b a partir do buffer
    memcpy(&(*btree)->rrn_root, &buffer[off], 4); off += 4;
    memcpy(&(*btree)->rrn_prox, &buffer[off], 4); off += 4;
    memcpy(&(*btree)->key_count, &buffer[off], 4); off += 4;

    return 0;
}

int dump_btree_cache(BTREE **btree, FILE *btree_fptr){
    CACHE_NODE *aux_node = (*btree)->cache->head->next;

    // Iteração por todos os nós da cache
    while(aux_node != (*btree)->cache->tail){
        // Escreve a página do nó no arquivo da árvore-b
        if(update_disk_page(aux_node->rrn, &aux_node->value, btree_fptr) == -1){
            // Houve algum erro guardando os valores da página em memória secundária
            return -1;
        }

        // Iterar para o próximo nó e limpeza do nó atual
        CACHE_NODE *node_temp = aux_node;
        aux_node = aux_node->next;
        free(node_temp);
    }

    // Reposicionamento dos ponteiros
    (*btree)->cache->head->next = (*btree)->cache->tail;
    (*btree)->cache->tail->prev = (*btree)->cache->head;

    return 0;
}

/**
 * @brief Busca de um índice em uma única página
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
    // Verifica se achou o elemento
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

    // Recuperação da página da busca
    if(!(page = get_page_readonly(rrn, btree, btree_fptr))){
        // Erro durante a leitura da página
        return -1;
    }

    // Variável que recebe a posição de onde fica ou deveria ficar o índice procurado na página atual
    int pos;
    int64_t res;

    // Achou o elemento na página
    if((res = page_search(index, &pos, page)) != -1){
        return res;
    }

    // Continuação da recursão
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
 * @brief Insere um registro em uma página não cheia (a função supõe que a página tenha espaço)
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
    // Página a ser dividida
    BTREE_PAGE *root_page;
    if(!(root_page = get_page(rrn_root, btree, btree_fptr))){
        return -1;
    }

    // Criação da nova página
    int32_t ret = (*btree)->rrn_prox;
    BTREE_PAGE *new_page;
    if(!(new_page = create_newpage_cache((*btree)->rrn_prox++, btree, btree_fptr))){
        return -1;
    }
    new_page->alt = root_page->alt;

    // Arrays temporários para a divisão dos registros (guarda os campos da página cheia mais o elemento a ser inserido)
    INDEX_REG temp_keys[BTREE_ORDER];
    int32_t temp_rrn_filhos[BTREE_ORDER+1];

    // Atribuição dos arrays temporários (cópia dos valores de root_page mais o elemento a ser inserido)

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

    // Atribuição do nó a ser promovido ao nível de cima

    int mid = (BTREE_ORDER-1)/2;
    (*promo_reg) = temp_keys[mid];


    // Atribuição dos valores da página criada (segunda metade dos arrays temporários depois de mid)

    new_page->nro_chaves = BTREE_ORDER - (mid+1);
    memcpy(new_page->chaves, &temp_keys[mid+1], sizeof(INDEX_REG) * new_page->nro_chaves);
    memcpy(new_page->rrn_filhos, &temp_rrn_filhos[mid+1], sizeof(int32_t) * (new_page->nro_chaves+1));


    // Atualização dos valores na página dividida (primeira metade dos arrays temporários antes de mid)

    for(int i = BTREE_ORDER - 2; i >= 0; i--){
        if(i >= mid){
            root_page->chaves[i] = (INDEX_REG){-1,-1};
            root_page->rrn_filhos[i+1] = -1;
        }
    }

    memcpy(root_page->chaves, temp_keys, sizeof(INDEX_REG) * mid);
    memcpy(root_page->rrn_filhos, temp_rrn_filhos, sizeof(int32_t) * (mid+1));

    root_page->nro_chaves = mid;

    return ret;
}

/**
 * @brief Componente recursivo de insert_btree
 * 
 * @param reg Registro a ser inserido
 * @param rrn_root RRN da página atual da iteração
 * @param promo_reg Registro promovido de um nível inferior para este nível
 * @param rrn_promo RRN da página de onde veio o registro a ser promovido
 * @param alt Altura da página atual
 * @param btree Referência para o ponteiro da árvore-b
 * @param btree_fptr Ponteiro para o arquivo da árvore-b
 *
 * @retval HAS_PROMO_KEY A inserção ocorreu com sucesso e há elementos a serem promovidos para um nível superior
 * @retval NO_PROMO_KEY A inserção ocorreu com sucesso e não há registros a serem promovidos para um nível superior
 * @retval -1 Houve algum erro durante a inserção do registro
 */
static int insert_btree_rec(const INDEX_REG reg, const int32_t rrn_root, INDEX_REG *promo_reg, int32_t *rrn_promo, int32_t *alt, BTREE **btree, FILE *btree_fptr){
    // Caso base
    if(rrn_root == -1){
        *promo_reg = reg;
        *rrn_promo = -1;
        (*btree)->key_count++;
        *alt = -1;
        return HAS_PROMO_KEY;
    }

    BTREE_PAGE *page;
    // Recuperação da página atual da recursão
    if (!(page = get_page(rrn_root, btree, btree_fptr))) {
        return -1;
    }

    // Procura pela posição ao qual o registro se encaixa

    int pos;
    page_search(reg.index, &pos, page);


    // Buffers para guardar valores para a promoção de um registro, caso houver

    INDEX_REG reg_promo_buff;
    int32_t rrn_promo_buff, alt_buff;

    // Chamada recursiva
    int ret = insert_btree_rec(reg, page->rrn_filhos[pos], &reg_promo_buff, &rrn_promo_buff, &alt_buff, btree, btree_fptr);

    // Acompanhamento da altura da iteração
    *alt = alt_buff+1;

    // Verificação se não há elementos a serem promovidos
    if(ret != HAS_PROMO_KEY){
        return ret;
    }

    // Novamente, a recuperação da página raiz da iteração (Com as chamadas recursivas, não há garantia da página raiz
    // ainda estar na cache)
    page = get_page(rrn_root, btree, btree_fptr);

    // Insere em uma página não cheia caso houver espaço
    if(has_space_page(page)){
        insert_page_nonfull(reg_promo_buff, pos, page, rrn_promo_buff);
        return NO_PROMO_KEY;
    }

    // Caso há a inserção em um nó cheio, dar split neste nó
    *rrn_promo = split_page(reg_promo_buff, rrn_promo_buff, pos, rrn_root, promo_reg, btree, btree_fptr);
    return HAS_PROMO_KEY;
}

int insert_btree(const INDEX_REG reg, BTREE **btree, FILE *btree_fptr){
    // Buffers para guardar valores para a promoção de um registro, caso houver

    INDEX_REG reg_promo_buff;
    int32_t rrn_promo_buff;
    int32_t alt_buff;

    // Chamada da componente recursiva de inserção
    int ret = insert_btree_rec(reg, (*btree)->rrn_root, &reg_promo_buff, &rrn_promo_buff, &alt_buff, btree, btree_fptr);

    // Caso o fim da recursão me retorne um elemnto a ser promovido, aumentar a altura da árvore-b
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
    }

    return ret;
}