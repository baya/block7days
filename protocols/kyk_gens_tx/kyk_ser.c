#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include "beej_pack.h"
#include "varint.h"
#include "kyk_utils.h"
#include "kyk_ser.h"


#define is_col(name) \
    strcmp(col, (name)) == 0 ? 1 : 0

#define VERSION_NO          "version-no"
#define IN_COUNTER          "in-counter"
#define PRE_TX_HASH         "pre-tx-hash"
#define PRE_TX_HASH_HEX     "pre-tx-hash:hex"
#define PRE_TXOUT_INX       "pre-txout-inx"
#define TXIN_SC_LEN         "txin-sc-len"
#define TXIN_SC_SIG         "txin-sc-sig"
#define TXIN_SC_SIG_HEX     "txin-sc-sig:hex"
#define SEQ_NO              "seq-no"
#define OUT_COUNTER         "out-counter"
#define TXOUT_SC_LEN        "txout-sc-len"
#define TXOUT_SC_PUBKEY_HEX "txout-sc-pubkey:hex"
#define TXOUT_VALUE         "txout-value"
#define LOCK_TIME           "lock-time"

static size_t kyk_tx_ser_version(uint8_t *buf, uint32_t val);
static size_t kyk_tx_ser_incounter(uint8_t *buf, varint_t val);
static size_t kyk_tx_ser_txout_inx(uint8_t *buf, uint32_t val);
size_t kyk_tx_ser_tx_hash(uint8_t *buf, const uint8_t *val, size_t val_len);
size_t kyk_tx_ser_tx_hash_hex(uint8_t *buf, const unsigned char *val);
size_t kyk_tx_ser_txin_sc_len(uint8_t *buf, varint_t val);
size_t kyk_ser_byte_val(uint8_t *buf, const uint8_t *val, size_t val_len);
size_t kyk_tx_ser_txin_sc_sig(uint8_t *buf, const uint8_t *val, size_t val_len);
size_t kyk_ser_byte_hex(uint8_t *buf, const unsigned char *val);
size_t kyk_ser_tx_seq_no(uint8_t *buf, uint32_t val);
size_t kyk_ser_txout_sc_len(uint8_t *buf, varint_t val);
size_t kyk_ser_tx_out_counter(uint8_t *buf, varint_t val);
size_t kyk_ser_txout_value(uint8_t *buf, uint64_t val);
size_t kyk_ser_tx_locktime(uint8_t *buf, uint32_t val);

size_t kyk_tx_ser(uint8_t *buf, char *col, ...)
{
    va_list ap;
    size_t len = 0;
    
    va_start(ap, col);
    
    if(is_col(VERSION_NO)){
	len += kyk_tx_ser_version(buf, va_arg(ap, uint32_t));
	
    } else if (is_col(IN_COUNTER)){
	len += kyk_tx_ser_incounter(buf, va_arg(ap, varint_t));
    } else if(is_col(PRE_TX_HASH)){
	len += kyk_tx_ser_tx_hash(buf, va_arg(ap, uint8_t*), va_arg(ap, size_t));
    } else if(is_col(PRE_TX_HASH_HEX)){
	len += kyk_tx_ser_tx_hash_hex(buf, va_arg(ap, unsigned char*));
    } else if(is_col(PRE_TXOUT_INX)) {
	len += kyk_tx_ser_txout_inx(buf, va_arg(ap, uint32_t));
    } else if(is_col(TXIN_SC_LEN)){
	len += kyk_tx_ser_txin_sc_len(buf, va_arg(ap, varint_t));
    } else if(is_col(TXIN_SC_SIG)){
	len += kyk_tx_ser_txin_sc_sig(buf, va_arg(ap, uint8_t*), va_arg(ap, size_t));
    } else if(is_col(TXIN_SC_SIG_HEX)){
	len += kyk_ser_byte_hex(buf, va_arg(ap, unsigned char*));
    } else if(is_col(SEQ_NO)){
	len += kyk_ser_tx_seq_no(buf, va_arg(ap, uint32_t));
    } else if(is_col(OUT_COUNTER)){
	len += kyk_ser_tx_out_counter(buf, va_arg(ap, varint_t));
    } else if(is_col(TXOUT_SC_LEN)){
	len += kyk_ser_txout_sc_len(buf, va_arg(ap, varint_t));
    } else if(is_col(TXOUT_SC_PUBKEY_HEX)){
	len += kyk_ser_byte_hex(buf, va_arg(ap, unsigned char*));
    } else if(is_col(TXOUT_VALUE)){
	len += kyk_ser_txout_value(buf, va_arg(ap, uint64_t));
    } else if(is_col(LOCK_TIME)){
	len += kyk_ser_tx_locktime(buf, va_arg(ap, uint32_t));
    } else {
	fprintf(stderr, "Invalid Tx col: %s\n", col);
    }

    va_end(ap);

    return len;
}

size_t kyk_tx_ser_version(uint8_t *buf, uint32_t val)
{
    size_t len = 0;
    len = beej_pack(buf, "<L", val);

    return len;
}

size_t kyk_tx_ser_incounter(uint8_t *buf, varint_t val)
{
    size_t len = 0;
    len = kyk_pack_varint(buf, val);

    return len;
}

size_t kyk_tx_ser_txout_inx(uint8_t *buf, uint32_t val)
{
    size_t len = 0;
    len = beej_pack(buf, "<L", val);

    return len;
}

size_t kyk_tx_ser_tx_hash(uint8_t *buf, const uint8_t *val, size_t val_len)
{
    size_t len = 0;
    len = kyk_ser_byte_val(buf, val, val_len);

    return len;
}

size_t kyk_tx_ser_tx_hash_hex(uint8_t *buf, const unsigned char *val)
{
    size_t len = 0;
    uint8_t *tmp;    

    tmp = kyk_alloc_hex((char*)val, &len);
    memcpy(buf, tmp, len * sizeof(uint8_t));

    free(tmp);
    
    return len;
}

size_t kyk_tx_ser_txin_sc_len(uint8_t *buf, varint_t val)
{
    size_t len = 0;
    len = kyk_pack_varint(buf, val);

    return len;
}

size_t kyk_tx_ser_txin_sc_sig(uint8_t *buf, const uint8_t *val, size_t val_len)
{
    size_t len = 0;
    len = kyk_ser_byte_val(buf, val, val_len);

    return len;
}

size_t kyk_ser_byte_val(uint8_t *buf, const uint8_t *val, size_t val_len)
{
    size_t len = 0;
    
    memcpy(buf, val, val_len * sizeof(uint8_t));
    len = val_len;

    return len;
}

size_t kyk_ser_byte_hex(uint8_t *buf, const unsigned char *val)
{
    size_t len = 0;
    uint8_t *tmp;    

    tmp = kyk_alloc_hex((char*)val, &len);
    memcpy(buf, tmp, len * sizeof(uint8_t));

    free(tmp);
    
    return len;
}

size_t kyk_ser_tx_seq_no(uint8_t *buf, uint32_t val)
{
    size_t len = 0;
    len = beej_pack(buf, ">L", val);

    return len;
}

size_t kyk_ser_tx_out_counter(uint8_t *buf, varint_t val)
{
    size_t len = 0;
    len = kyk_pack_varint(buf, val);

    return len;
}

size_t kyk_ser_txout_sc_len(uint8_t *buf, varint_t val)
{
    size_t len = 0;
    len = kyk_pack_varint(buf, val);

    return len;
}

size_t kyk_ser_txout_value(uint8_t *buf, uint64_t val)
{
    size_t len;
    len = beej_pack(buf, ">Q", val);

    return len;
}

size_t kyk_ser_tx_locktime(uint8_t *buf, uint32_t val)
{
    size_t len;
    len = beej_pack(buf, "<L", val);

    return len;
}


