#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <openssl/pem.h>

#include "kyk_tx.h"
#include "kyk_sha.h"
#include "kyk_utils.h"
#include "kyk_script.h"
#include "kyk_address.h"

#define SC_PUBKEY_LEN 1000
#define KYK_TX_BUF_LEN 10000

int get_priv_from_pem(uint8_t *priv, const char *pem_file_name);

int main()
{
    uint8_t tx_buf[KYK_TX_BUF_LEN];
    size_t tx_buf_len;
    
    struct kyk_tx tx0;
    struct kyk_txin *txin;
    struct kyk_txout *txout;

    char *cb = "4/Sept/2017 China start forbiding bitcoin";
    
    uint8_t priv[32];
    char *pem_name = "kyk-gens-priv.pem";
    char *addr;
    uint8_t sc_pbk[SC_PUBKEY_LEN];
    size_t sc_pbk_len;

    FILE *fp = fopen("gens-tx.bin", "wb");
    size_t wsize;


    tx0.version = 1;
    tx0.vin_sz = 1;
    tx0.lock_time = 0;
    tx0.txin = malloc(tx0.vin_sz * sizeof(struct kyk_txin));
    tx0.vout_sz = 1;
    tx0.txout = malloc(tx0.vout_sz * sizeof(struct kyk_txout));
    txin = tx0.txin;
    txout = tx0.txout;

    memset(txin -> pre_txid, 0x00, sizeof(txin -> pre_txid));
    txin -> pre_tx_inx = 0;
    txin -> sc_size = strlen(cb);
    
    txin -> sc = (unsigned char *) malloc(txin -> sc_size * sizeof(unsigned char));
    
    memcpy(txin -> sc, cb, txin -> sc_size);
    
    txin -> seq_no = 0xFFFFFFFF;

    txout -> value = 100;
    

    get_priv_from_pem(priv, pem_name);
    addr = kyk_make_address(priv);
    sc_pbk_len = p2pkh_sc_from_address(sc_pbk, addr);
    txout -> sc_size = sc_pbk_len;
    txout -> sc = sc_pbk;

    tx_buf_len = kyk_seri_tx(tx_buf, &tx0);

    kyk_print_hex("tx0 ", tx_buf, tx_buf_len);

    wsize = fwrite(tx_buf, sizeof(tx_buf[0]), tx_buf_len, fp);
    printf("wsize:%lu, tx_buf_len:%lu\n", wsize, tx_buf_len);
    
}

int get_priv_from_pem(uint8_t *priv, const char *pem_file_name)
{
    EVP_PKEY *evp_key;
    EC_KEY *ec_key;
    const BIGNUM *priv_bn;
    char *addr;

    FILE *fp = fopen(pem_file_name, "r");
    if(!fp){
	perror("Pem File opening failed");
        return EXIT_FAILURE;
    }
    evp_key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    if (!evp_key)	{
	fprintf(stderr, "Unable to read pem\n");
	return -1;
    }

    ec_key = EVP_PKEY_get1_EC_KEY(evp_key);
    priv_bn = EC_KEY_get0_private_key(ec_key);
    BN_bn2bin(priv_bn, priv);

    EC_KEY_free(ec_key);
    EVP_PKEY_free(evp_key);
    fclose(fp);

    return 1;
}
