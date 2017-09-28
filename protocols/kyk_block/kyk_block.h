#ifndef KYK_BLOCK_H__
#define KYK_BLOCK_H__

#include "varint.h"
#include "kyk_tx.h"

struct kyk_blk_header {
    uint32_t version;
    uint8_t pre_blk_hash[32];
    uint8_t mrk_root_hash[32];
    uint32_t tts;
    uint32_t bts;
    uint32_t nonce;
};

struct kyk_block {
    uint32_t magic_no;
    uint32_t blk_size;
    struct kyk_blk_header *hd;
    varint_t tx_count;
    struct kyk_tx *tx;
};

size_t kyk_seri_blk_hd(uint8_t *buf, const struct kyk_blk_header *hd);

#endif
