#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "kyk_sha.h"

unsigned char * kyk_sha256(const char *str)
{
    unsigned char *dgst;
    SHA256_CTX ctx;

    dgst = (unsigned char*)malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char));
    
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (unsigned char*)str, 80);
    SHA256_Final(dgst, &ctx);

    return dgst;
}


unsigned char * kyk_dble_sha256(const char *str)
{
    unsigned char *dg1;
    unsigned char *dg2;
    
    dg1 = kyk_sha256(str);
    dg2 = kyk_sha256((char *)dg1);
    
    //free(dg1);

    return dg2;
}









