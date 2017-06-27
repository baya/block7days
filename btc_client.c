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

typedef struct btc_tx_input {
  uint8_t pre_txout_hash[32];
  uint32_t pre_txout_inx;
  btc_varint sc_len;
  uint8_t *sc_sig_ptr;
  uint32_t seq_n;
} btc_txin;

typedef struct btc_tx_out {
  uint64_t value;
  btc_varint sc_len;
  uint8_t *sc_pbk_ptr;
} btc_txout;

typedef struct btc_transaction {
  uint32_t ver;
  btc_varint vin_sz;
  btc_txin *txin_ptr;
  btc_varint vout_sz;
  btc_txout *txout_ptr;
} btc_tx;

void encode_btc_varint(btc_varint *vt, uint64_t i);
void hash_strtobin(uint8_t *hb_ptr, size_t len, const char *str);
void print_bin_to_hex(uint8_t *bytes, size_t len);


int main(int argc, char *argv[])
{
  btc_tx tx1;
  btc_varint vin_sz;
  btc_txin txin1;
  const char *pre_tx_hash = "f4184fc596403b9d638783cf57adfe4c75c605f6356fbc91338530e9831e9e16";
  
  encode_btc_varint(&vin_sz, 1);
  tx1.ver = 1;
  tx1.vin_sz = vin_sz;
  hash_strtobin(txin1.pre_txout_hash, 32, pre_tx_hash);
  txin1.pre_txout_inx = 1;

  printf("ver: %u\n", tx1.ver);
  printf("vin_sz: %u\n", tx1.vin_sz.va1);
  printf("Txin prev out hash: ");
  print_bin_to_hex(txin1.pre_txout_hash, 32);
  printf("\n");
  
}

void print_bin_to_hex(uint8_t *bytes, size_t len)
{
  for(int i=0; i < len; i++){
    printf("%02x", bytes[i]);
  }
}

void hash_strtobin(uint8_t *hb_ptr, size_t len, const char *str)
{
  size_t count = 0;
  for(count = 0; count < len/sizeof(hb_ptr[0]); count++) {
    sscanf(str, "%2hhx", hb_ptr);
    hb_ptr++;
    str += 2;
  }  
}

void encode_btc_varint(btc_varint *vt, uint64_t i)
{
  vt->va1 = 0;
  vt->va2 = 0;
  vt->va4 = 0;
  vt->va8 = 0;
  
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

