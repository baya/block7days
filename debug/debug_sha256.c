/*
 ** gcc debug_sha256.c -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lcrypto
 **
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

unsigned char * kyk_sha256(const unsigned char *str);

int main(void)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const char* string = "";
    unsigned char digest2[SHA256_DIGEST_LENGTH];
    const char *str2 = "hello world";
    unsigned char *dg1;
    unsigned char *dg2;

    printf("+++++++%u\n", SHA256_DIGEST_LENGTH);

    SHA256_CTX ctx;
    SHA256_CTX ctx2;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, string, strlen(string));
    SHA256_Final(digest, &ctx);

    SHA256_Init(&ctx2);
    SHA256_Update(&ctx2, digest, strlen((char*) digest));
    SHA256_Final(digest2, &ctx2);

    dg1 = kyk_sha256((unsigned char*) str2);
    dg2 = kyk_sha256(dg1);
    free(dg1);

    char mdString[SHA256_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++){
	printf("%02x", digest[i]);
    }
    printf("\n");

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++){
	printf("%02x", digest2[i]);
    }
    printf("\n");

    printf("SHA256 digest: %s\n", mdString);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++){
	printf("%02x", dg1[i]);
    }
    printf("\n");

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++){
	printf("%02x", dg2[i]);
    }
    printf("\n");
    
}


unsigned char * kyk_sha256(const unsigned char *str)
{
    unsigned char *dgst;
    SHA256_CTX ctx;

    dgst = (unsigned char *) malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char));
    
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, str, strlen((char *)str));
    SHA256_Final(dgst, &ctx);

    return dgst;
}
