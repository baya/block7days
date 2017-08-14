#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "kyk_sha.h"

unsigned char * kyk_sha256(const char *str, size_t len)
{
    unsigned char *dgst;
    SHA256_CTX ctx;

    dgst = (unsigned char*)malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char));
    
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (unsigned char*)str, len);
    SHA256_Final(dgst, &ctx);

    return dgst;
}


unsigned char * kyk_dble_sha256(const char *str, size_t len)
{
    unsigned char *dg1;
    unsigned char *dg2;
    
    dg1 = kyk_sha256(str, len);
    dg2 = kyk_sha256((char *)dg1, SHA256_DIGEST_LENGTH);
    
    free(dg1);

    return dg2;
}









