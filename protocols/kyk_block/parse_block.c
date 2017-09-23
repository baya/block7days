#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "varint.h"
#include "kyk_tx.h"

struct kyk_blk_header {
    uint32_t version;
    uint8_t pre_blk_hash[32];
    uint8_t mrk_root_hash[32];
    uint32_t time;
    uint32_t bts;
    uint8_t nonce[32];
};

struct kyk_block {
    uint32_t magic_no;
    uint32_t blk_size;
    struct kyk_blk_header *hder;
    varint_t tx_count;
    struct kyk_tx *tx;
};

void kyk_parse_blk(struct kyk_block *blk, FILE *fp);
void kyk_print_blk(struct kyk_block *blk);

int main(int argc, char *argv[])
{
    FILE *fp;
    struct kyk_block blk;
    
    fp = fopen(argv[1], "rb");
    kyk_parse_blk(&blk, fp);
    kyk_print_blk(&blk);
}

void kyk_parse_blk(struct kyk_block *blk, FILE *fp)
{
    fread(&blk -> magic_no, sizeof(blk -> magic_no), 1, fp);
    fread(&blk -> blk_size, sizeof(blk -> blk_size), 1, fp);
}

void kyk_print_blk(struct kyk_block *blk)
{
    printf("Magic no: %x\n", blk -> magic_no);
    printf("Blocksize: %x\n", blk -> blk_size);
}





