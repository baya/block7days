#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kyk_tx.h"
#include "varint.h"
#include "beej_pack.h"


static size_t kyk_seri_txin(unsigned char *buf, struct kyk_txin *txin);
static size_t reverse_pack_chars(unsigned char *buf, unsigned char *src, size_t count);
static size_t kyk_seri_txin_list(unsigned char *buf, struct kyk_txin *txin, size_t count);
static size_t pack_chars(unsigned char *buf, unsigned char *src, size_t count);
static size_t kyk_seri_txout(unsigned char *buf, struct kyk_txout *txout);
size_t kyk_seri_txout_list(unsigned char *buf, struct kyk_txout *txout, size_t count);

size_t kyk_seri_tx(unsigned char *buf, struct kyk_tx *tx)
{
    size_t size;
    size_t total = 0;
    struct kyk_txin *txin_ptr;

    size = beej_pack(buf, "<L", tx -> version);
    buf += size;
    total += size;

    size = kyk_pack_varint(buf, tx -> vin_sz);
    buf += size;
    total += size;

    size = kyk_seri_txin_list(buf, tx -> txin, tx -> vin_sz);
    buf += size;
    total += size;

    size = kyk_pack_varint(buf, tx -> vout_sz);
    buf += size;
    total += size;

    size = kyk_seri_txout_list(buf, tx -> txout, tx -> vout_sz);
    buf += size;
    total += size;

    size = beej_pack(buf, "<L", tx -> lock_time);
    buf += size;
    total += size;

    return total;
}

size_t kyk_seri_txin_list(unsigned char *buf, struct kyk_txin *txin, size_t count)
{
    size_t size;
    size_t total = 0;

    for(int i=0; i < count; i++){
	size = kyk_seri_txin(buf, txin);
	txin++;
	buf += size;
	total += size;
    }

    return total;
}

size_t kyk_seri_txout_list(unsigned char *buf, struct kyk_txout *txout, size_t count)
{
    size_t size;
    size_t total = 0;

    for(int i=0; i < count; i++){
	size = kyk_seri_txout(buf, txout);
	txout++;
	buf += size;
	total += size;
    }

    return total;
}


size_t kyk_seri_txin(unsigned char *buf, struct kyk_txin *txin)
{
    size_t size;
    size_t total = 0;

    size = reverse_pack_chars(buf, txin -> pre_txid, sizeof(txin->pre_txid));
    buf += size;
    total += size;

    size = beej_pack(buf, "<L", txin -> pre_tx_inx);
    buf += size;
    total += size;

    size = kyk_pack_varint(buf, txin -> sc_size);
    buf += size;
    total += size;

    size = pack_chars(buf, txin -> sc, txin -> sc_size);
    buf += size;
    total += size;

    size = beej_pack(buf, "<L", txin -> seq_no);
    buf += size;
    total += size;

    return total;
}

size_t kyk_seri_txout(unsigned char *buf, struct kyk_txout *txout)
{
    size_t size;
    size_t total = 0;

    size = beej_pack(buf, "<Q", txout -> value);
    buf += size;
    total += size;

    size = kyk_pack_varint(buf, txout -> sc_size);
    buf += size;
    total += size;

    size = pack_chars(buf, txout -> sc, txout -> sc_size);
    buf += size;
    total += size;

    return total;
}

size_t pack_chars(unsigned char *buf, unsigned char *src, size_t count)
{
    size_t size = 0;

    for(int i=0; i < count; i++){
	*buf = src[i];
	buf++;
	size += 1;
    }

    return size;
}

size_t reverse_pack_chars(unsigned char *buf, unsigned char *src, size_t count)
{
    size_t size = 0;

    for(int i=count-1; i >= 0; i--){
	*buf = src[i];
	buf++;
	size += 1;
    }

    return size;
}
