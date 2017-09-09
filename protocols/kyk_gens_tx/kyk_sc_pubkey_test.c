#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <openssl/ripemd.h>
#include <assert.h>

#include "kyk_base58.h"
#include "kyk_utils.h"

int main()
{
    BIGNUM bn;
    char *addr = "1KFHE7w8BhaENAswwryaoccDb6qcT6DbYY";
    size_t len;
    uint8_t buf[1 + RIPEMD160_DIGEST_LENGTH + 4];
    uint8_t pubk_hash[RIPEMD160_DIGEST_LENGTH];
    uint8_t csum[4];

    BN_init(&bn);
    raw_decode_base58(&bn, addr, strlen(addr));

    len = BN_num_bytes(&bn);
    memset(buf, 0, sizeof(buf));
    BN_bn2bin(&bn, buf + sizeof(buf) - len);

    BN_free(&bn);

    assert(validate_base58_checksum(buf, 1+RIPEMD160_DIGEST_LENGTH) > 0);

    kyk_print_hex("raw decoded ", buf, sizeof(buf));
    base58_get_checksum(csum, buf, 1+RIPEMD160_DIGEST_LENGTH);
    kyk_print_hex("checksum ", csum, 4);
    memcpy(pubk_hash, buf+1, sizeof(pubk_hash));
    kyk_print_hex("pubKeyHash ", buf+1, sizeof(pubk_hash));
}
