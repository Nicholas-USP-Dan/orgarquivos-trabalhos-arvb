#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "index-btree.h"
#include "index-btree-internals.h"

#include "index-btree-utils.h"

int read_disk_page(BTREE_PAGE *const page, const int32_t rrn, FILE *btree_fptr){
    fseek(btree_fptr, (rrn+1) * BTREE_PAGE_SIZE, SEEK_SET);

    // Leitura da página inteira em disco
    unsigned char buffer[BTREE_PAGE_SIZE];
    if(fread(buffer, sizeof(unsigned char), BTREE_PAGE_SIZE, btree_fptr) != BTREE_PAGE_SIZE){
        // Houve algum erro durante a leitura do bloco
        return -1;
    }

    // Transcrição do buffer para uma estrutura de página
    int off = 0;

    memcpy(&page->alt, &buffer[off], 4); off += 4;
    memcpy(&page->nro_chaves, &buffer[off], 4); off += 4;

    for(int i = 0; i < BTREE_ORDER-1; i++){
        memcpy(&page->chaves[i].index, &buffer[off], 4); off += 4;
        memcpy(&page->chaves[i].offset, &buffer[off], 8); off += 8;
    }

    memcpy(page->rrn_filhos, &buffer[off], sizeof(int32_t) * BTREE_ORDER);
    off += sizeof(int32_t) * BTREE_ORDER;

    return 0;
}

int update_disk_page(const int32_t rrn, const BTREE_PAGE *page, FILE *btree_fptr){
    fseek(btree_fptr, (rrn+1) * BTREE_PAGE_SIZE, SEEK_SET);
    
    // Bloco a ser escrito em disco
    unsigned char buffer[BTREE_PAGE_SIZE];

    // Escrita da estrutura da página para o buffer
    int off = 0;
    memcpy(&buffer[off], &page->alt, 4); off += 4;
    memcpy(&buffer[off], &page->nro_chaves, 4); off += 4;

    for(int i = 0; i < BTREE_ORDER-1; i++){
        memcpy(&buffer[off], &page->chaves[i].index, 4); off += 4;
        memcpy(&buffer[off], &page->chaves[i].offset, 8); off += 8;
    }

    memcpy(&buffer[off], page->rrn_filhos,  sizeof(int32_t) * BTREE_ORDER);
    off += sizeof(int32_t) * BTREE_ORDER;

    // Escrita do buffer no arquivo
    if(fwrite(buffer, sizeof(unsigned char), BTREE_PAGE_SIZE, btree_fptr) != BTREE_PAGE_SIZE){
        // Houve algum erro durante a escrita do bloco
        perror("Erro de escrita: ");
        return -1;
    }

    return 0;
}