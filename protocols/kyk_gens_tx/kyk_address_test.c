#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "kyk_ecdsa.h"
#include "kyk_utils.h"

int main()
{
    /*
     * 0 - Having a private ECDSA key
     */
    uint8_t priv_bytes[32] = {
	0x18,0xE1,0x4A,0x7B,0x6A,0x30,0x7F,0x42,
	0x6A,0x94,0xF8,0x11,0x47,0x01,0xE7,0xC8,
	0xE7,0x74,0xE7,0xF9,0xA4,0x7E,0x2C,0x20,
	0x35,0xDB,0x29,0xA2,0x06,0x32,0x17,0x25
    };

    EC_KEY *key;
    uint8_t priv[32];
    const BIGNUM *priv_bn;
    point_conversion_form_t conv_form = POINT_CONVERSION_UNCOMPRESSED;
    size_t pub_len;
    uint8_t *pub, *pub_copy;


    key = kyk_ec_new_keypair(priv_bytes);
    if (!key) {
        fprintf(stderr, "Unable to create keypair\n");
        return -1;
    }

    priv_bn = EC_KEY_get0_private_key(key);
    if (!priv_bn) {
        fprintf(stderr, "Unable to decode private key\n");
        return -1;
    }
    BN_bn2bin(priv_bn, priv);
    kyk_print_hex("0 - Having a private ECDSA key        ", priv, sizeof(priv));


    BN_bn2bin(priv_bn, priv);
    EC_KEY_set_conv_form(key, conv_form);
    pub_len = i2o_ECPublicKey(key, NULL);
    pub = calloc(pub_len, sizeof(uint8_t));
    pub_copy = pub;
    
    /*
     * 1 - Take the corresponding public key generated with it
     *     (
     *       65 bytes, 1 byte 0x04,
     *       32 bytes corresponding to X coordinate,
     *       32 bytes corresponding to Y coordinate
     *     )
     *
     */
    if (i2o_ECPublicKey(key, &pub_copy) != pub_len) {
	fprintf(stderr, "Unable to decode public key\n");
	return -1;
    }

    kyk_print_hex("1 - Take the corresponding public key ", pub, pub_len);


    EC_KEY_free(key);
    
    return 0;
}
