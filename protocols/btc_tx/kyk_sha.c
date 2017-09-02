#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "kyk_sha.h"

static void print_bytes_in_hex(const unsigned char *buf, size_t len);

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


/* inverted hash*/
struct kyk_ivhash *kyk_inver_hash(const char *src, size_t len)
{
    unsigned char *dg;
    struct kyk_ivhash *ivhash;
    size_t dg_len = SHA256_DIGEST_LENGTH;

    dg = kyk_dble_sha256(src, len);
    ivhash = (struct kyk_ivhash*) malloc(sizeof(struct kyk_ivhash));
    ivhash -> len = dg_len;
    ivhash -> body = (unsigned char*)malloc(dg_len * sizeof(unsigned char));
    
    if(ivhash -> body == NULL){
	fprintf(stderr, "failed in malloc kyk inver hash\n");
	exit(1);
    }

    for(int i=dg_len -1; i >= 0; i--){
	ivhash->body[dg_len - 1 - i] = dg[i];
    }


    free(dg);

    return ivhash;
}



void print_bytes_in_hex(const unsigned char *buf, size_t len)
{
    for(int i=0; i < len; i++){
	printf("%02x", buf[i]);
    }
    printf("\n");
}




