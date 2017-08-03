/*
 ** gcc debug_sha256.c -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lcrypto -lssl
 **
 */
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

int main(void)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const char* string = "hello world";

    printf("+++++++%u\n", SHA256_DIGEST_LENGTH);
}
