#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "kyk_base58.h"
#include "kyk_utils.h"

int main()
{
    BIGNUM bn;
    char *addr = "1KFHE7w8BhaENAswwryaoccDb6qcT6DbYY";
    size_t len;
    uint8_t buf[1 + 32 + 4];

    BN_init(&bn);
    raw_decode_base58(&bn, addr, strlen(addr));

    len = BN_num_bytes(&bn);
    memset(buf, 0, sizeof(buf));
    BN_bn2bin(&bn, buf + sizeof(buf) - len);

    kyk_print_hex("pubkey ", buf, sizeof(buf));
}
