#ifndef __KYK_BASE58_H
#define __KYK_BASE58_H


char *kyk_base58(const uint8_t *bytes, size_t len);
char *kyk_base58check(const uint8_t *bytes, size_t len);

#endif
