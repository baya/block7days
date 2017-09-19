#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

#include "kyk_tx.h"
#include "kyk_sha.h"
#include "kyk_utils.h"
#include "kyk_script.h"


int main()
{
    struct kyk_tx tx0;
    struct kyk_txin txin;
    struct kyk_txout txout;

    char *cb = "4/Sept/2017 China start forbiding bitcoin";

    tx0.version = 1;
    tx0.vin_sz = 1;
    tx0.lock_time = 0;

    memset(txin.pre_txid, 0x00, sizeof(txin.pre_txid));
    txin.pre_tx_inx = 0;
    txin.sc_size = strlen(cb);
    txin.sc = (unsigned char *) malloc(txin.sc_size * sizeof(unsigned char));
    memcpy(txin.sc, cb, txin.sc_size);
    txin.seq_no = 0xFFFFFFFF;

    txout.value = 100;
    
}
