#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "kyk_sha.h"

unsigned char * kyk_sha256(const char *str)
{
    unsigned char *dgst;
    SHA256_CTX ctx;

    dgst = (unsigned char*)malloc(SHA256_DIGEST_LENGTH * sizeof(char));
    
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, str, strlen((char *)str));
    SHA256_Final(dgst, &ctx);

    return dgst;
}








