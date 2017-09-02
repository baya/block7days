#ifndef KYK_SHA_H__
#define KYK_SHA_H__

struct kyk_hash{
    size_t len;
    unsigned char *body;
};

unsigned char * kyk_sha256(const char *str, size_t len);
unsigned char * kyk_dble_sha256(const char *str, size_t len);
struct kyk_hash *kyk_inver_hash(const char *src, size_t len);

#endif
