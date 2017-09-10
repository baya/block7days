#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "kyk_base58.h"
#include "kyk_script.h"
#include "kyk_utils.h"

static size_t build_p2pkh_sc_pubk(unsigned char *buf, const unsigned char *pkh, size_t pkh_len);
static int pubk_hash_from_address(unsigned char *pubk_hash, size_t pkh_len, const char *addr);

size_t p2pkh_sc_from_address(unsigned char *sc, const char *addr)
{
    uint8_t pubk_hash[RIPEMD160_DIGEST_LENGTH];
    size_t len;
    
    if(pubk_hash_from_address(pubk_hash, sizeof(pubk_hash), addr) < 0){
	return -1;
    }
    
    len = build_p2pkh_sc_pubk(sc, pubk_hash, sizeof(pubk_hash));

    return len;
}

int pubk_hash_from_address(unsigned char *pubk_hash, size_t pkh_len, const char *addr)
{
    BIGNUM bn;
    size_t len;
    uint8_t buf[1 + RIPEMD160_DIGEST_LENGTH + 4];
    uint8_t csum[4];

    BN_init(&bn);
    raw_decode_base58(&bn, addr, strlen(addr));

    len = BN_num_bytes(&bn);
    memset(buf, 0, sizeof(buf));
    BN_bn2bin(&bn, buf + sizeof(buf) - len);

    BN_free(&bn);

    if(validate_base58_checksum(buf, 1 + RIPEMD160_DIGEST_LENGTH) < 0){
	fprintf(stderr, "address base58 checksum failed\n");
	return -1;
    }

    memcpy(pubk_hash, buf+1, pkh_len);

    return 1;

}

/* 76       A9             14                                                        */
/* OP_DUP OP_HASH160    Bytes to push                                                */
/* 89 AB CD EF AB BA AB BA AB BA AB BA AB BA AB BA AB BA AB BA   88         AC       */
/*                       Data to push                     OP_EQUALVERIFY OP_CHECKSIG */
size_t build_p2pkh_sc_pubk(unsigned char *sc, const unsigned char *pkh, size_t pkh_len)
{
    size_t count = 0;
    
    *sc = OP_DUP;
    count += 1;

    sc++;
    *sc = OP_HASH160;
    count += 1;

    sc++;
    *sc = (uint8_t) pkh_len;
    count += 1;

    sc++;
    memcpy(sc, pkh, pkh_len);
    count += pkh_len;
    
    sc += pkh_len;
    *sc = OP_EQUALVERIFY;
    count += 1;

    sc++;
    *sc = OP_CHECKSIG;
    count += 1;

    return count;
}


