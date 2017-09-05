#ifndef __KYK_ECDSA_H
#define __KYK_ECDSA_H

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>

EC_KEY *kyk_ec_new_keypair(const uint8_t *priv_bytes);
EC_KEY *kyk_ec_new_pubkey(const uint8_t *pub_bytes, size_t pub_len);

#endif
