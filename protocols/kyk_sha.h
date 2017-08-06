#ifndef KYK_SHA_H__
#define KYK_SHA_H__

unsigned char * kyk_sha256(const char *str, size_t len);
unsigned char * kyk_dble_sha256(const char *str, size_t len);

#endif
