#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t  V_PAD1 = 0xfd;
static const uint8_t  V_PAD2 = 0xfe;
static const uint8_t  V_PAD3 = 0xff;

static const uint8_t  N1 = 0xff;
static const uint16_t N2 = 0xffff;
static const uint32_t N4 = 0xffffffff;
static const uint64_t N8 = 0xffffffffffffffff;

typedef struct btc_varint {
  uint8_t  va1;
  uint16_t va2;
  uint32_t va4;
  uint64_t va8;
} btc_varint;

typedef struct btc_coinbase_sig{
  btc_varint len;
  uint8_t *sig;
} btc_cb_sig;

typedef struct btc_txin_sc_sig {
  btc_varint len;
  uint8_t *sig;
  uint8_t *pubkey;
} btc_sc_sig;

typedef struct btc_tx_input {
  uint8_t pre_tx_hash[32];
  uint32_t pre_txout_inx;
  btc_varint txin_sc_len;
} btc_txin;

typedef struct btc_transaction {
  uint32_t version_no;
  btc_varint vin_sz;
} btc_tx;

void encode_btc_varint(btc_varint *vt, uint64_t i);


int main(int argc, char *argv[])
{
  btc_tx tx0;
  btc_varint vin_sz;
  
  encode_btc_varint(&vin_sz, 1);
  tx0.version_no = 1;
  tx0.vin_sz = vin_sz;

  printf("ver: %u\n", tx0.version_no);
  printf("vin_sz: %u\n", tx0.vin_sz.va1);
}

void encode_btc_varint(btc_varint *vt, uint64_t i)
{
  if(i < V_PAD1){
    vt->va1 = i;
  } else if(i >= V_PAD1 && i <= N2){
    vt->va1 = V_PAD1;
    vt->va2 = i;
  } else if(i >= V_PAD2 && i <= N4){
    vt->va1 = V_PAD2;
    vt->va4 = i;
  } else if(i >= V_PAD3 && i <= N8){
    vt->va1 = V_PAD3;
    vt->va8 = i;
  }
}

