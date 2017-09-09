#ifndef __KYK_BASE58_H
#define __KYK_BASE58_H

#include <openssl/bn.h>

char *kyk_base58(const uint8_t *bytes, size_t len);
char *kyk_base58check(const uint8_t *bytes, size_t len);
int raw_decode_base58(BIGNUM *bn, const char *src, size_t len);

#endif
