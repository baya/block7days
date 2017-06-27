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
  uint64_t value;
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
void strtobin(uint8_t *b_ptr, size_t len, const char *str);
void hash_strtobin(uint8_t *hb_ptr, const char *str);
void print_bin_to_hex(uint8_t *bytes, size_t len);
void fill_in_txin(btc_txin *txin);
void scsig_strtobin(btc_txin *txin, const char *str);
void fill_in_tx(btc_tx *tx);
void fill_in_txout(btc_txout *txout);
void scpbk_strtobin(btc_txout *txout, const char *str);


int main(int argc, char *argv[])
{
  btc_tx tx1;
  btc_varint vin_sz;
  btc_txin txin1;
  btc_txout *txout;
  const char *pre_tx_hash = "f4184fc596403b9d638783cf57adfe4c75c605f6356fbc91338530e9831e9e16";
  const char *sc_sig = "473044022027542a94d6646c51240f23a76d33088d3dd8815b25e9ea18cac67d1171a3212e02203baf203c6e7b80ebd3e588628466ea28be572fe1aaa3f30947da4763dd3b3d2b01";
  const char *sc_pbk1 = "4104b5abd412d4341b45056d3e376cd446eca43fa871b51961330deebd84423e740daa520690e1d9e074654c59ff87b408db903649623e86f1ca5412786f61ade2bfac";
  const char *sc_pbk2 = "410411db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3ac";
  
  encode_btc_varint(&vin_sz, 1);
  tx1.ver = 1;
  tx1.vin_sz = vin_sz;
  tx1.txin_ptr = &txin1;
  hash_strtobin(txin1.pre_txout_hash, pre_tx_hash);
  txin1.pre_txout_inx = 1;
  encode_btc_varint(&txin1.sc_len, 72);
  fill_in_txin(&txin1);
  scsig_strtobin(&txin1, sc_sig);

  encode_btc_varint(&tx1.vout_sz, 2);
  fill_in_tx(&tx1);

  txout = &tx1.txout_ptr[0];
  txout -> value = 1000000000;
  encode_btc_varint(&txout->sc_len, 67);
  fill_in_txout(txout);
  scpbk_strtobin(txout, sc_pbk1);

  txout = &tx1.txout_ptr[1];
  txout -> value = 3000000000;
  encode_btc_varint(&txout->sc_len, 67);
  fill_in_txout(txout);
  scpbk_strtobin(txout, sc_pbk2);


  printf("ver: %u\n", tx1.ver);
  printf("vin_sz: %u\n", tx1.vin_sz.va1);
  printf("Txin prev out hash: ");
  print_bin_to_hex(txin1.pre_txout_hash, 32);
  printf("\n");
  printf("Txin prev out inx: %u\n", tx1.txin_ptr -> pre_txout_inx);
  printf("Txin Script Sig: ");
  print_bin_to_hex(tx1.txin_ptr[0].sc_sig_ptr, tx1.txin_ptr[0].sc_len.value);
  printf("\n");

  printf("vout_sz: %u\n", tx1.vout_sz.va1);
  printf("Txout Value: %llu\n", tx1.txout_ptr[0].value);
  printf("Txout Script PubKey: ");
  print_bin_to_hex(tx1.txout_ptr[0].sc_pbk_ptr, tx1.txout_ptr[0].sc_len.value);
  printf("\n");

  printf("Txout Value: %llu\n", tx1.txout_ptr[1].value);
  printf("Txout Script PubKey: ");
  print_bin_to_hex(tx1.txout_ptr[1].sc_pbk_ptr, tx1.txout_ptr[1].sc_len.value);
  printf("\n");
  
}

void print_bin_to_hex(uint8_t *bytes, size_t len)
{
  for(int i=0; i < len; i++){
    printf("%02x", bytes[i]);
  }
}

void strtobin(uint8_t *b_ptr, size_t len, const char *str)
{
  size_t count = 0;
  for(count = 0; count < len/sizeof(b_ptr[0]); count++)
  {
    sscanf(str, "%2hhx", b_ptr);
    b_ptr++;
    str += 2;
  }
}

void fill_in_tx(btc_tx *tx)
{
  size_t vout_sz = tx -> vout_sz.value;
  tx -> txout_ptr = malloc(vout_sz * sizeof(*tx -> txout_ptr));
}

void fill_in_txout(btc_txout *txout)
{
  txout -> sc_pbk_ptr = malloc(txout -> sc_len.value * sizeof(*txout -> sc_pbk_ptr));
}

void fill_in_txin(btc_txin *txin)
{
  txin -> sc_sig_ptr = malloc(txin -> sc_len.value * sizeof(*txin -> sc_sig_ptr));
}

void scpbk_strtobin(btc_txout *txout, const char *str)
{
  uint8_t *sc_ptr = txout -> sc_pbk_ptr;
  size_t len = txout -> sc_len.value;
  
  strtobin(sc_ptr, len, str);
}

void scsig_strtobin(btc_txin *txin, const char *str)
{
  uint8_t *scsig_ptr = txin -> sc_sig_ptr;
  size_t len = txin -> sc_len.value;
  
  strtobin(scsig_ptr, len, str);
}

void hash_strtobin(uint8_t *hb_ptr, const char *str)
{
  size_t count = 0;
  size_t len = 32;
  strtobin(hb_ptr, len, str);
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

  vt->value = i;
}

