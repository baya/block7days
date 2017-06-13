#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define COINBASE_INX       0xffffffff
#define NULL_HASH          "0000000000000000000000000000000000000000000000000000000000000000"

#define OP_FALSE           0x00
#define OP_PUSHDATA0_START 0x01
#define OP_PUSHDATA0_END   0x4b
#define OP_PUSHDATA1       0x4c
#define OP_PUSHDATA2       0x4d
#define OP_PUSHDATA4       0x4e
#define OP_1NEGATE         0x4f
#define OP_TRUE            0x51
#define OP_2               0x52
#define OP_3               0x53
#define OP_4               0x54
#define OP_5               0x55
#define OP_6               0x56
#define OP_7               0x57
#define OP_8               0x58
#define OP_9               0x59
#define OP_10              0x5a
#define OP_11              0x5b
#define OP_12              0x5c
#define OP_13              0x5d
#define OP_14              0x5e
#define OP_15              0x5f
#define OP_16              0x60

#define SIG_BUF_SIZE       1000

typedef struct coinbase_sig {
  char sig[SIG_BUF_SIZE];
  uint64_t len;
} coinbase_sig;

typedef struct script_sig {
  char sig[SIG_BUF_SIZE];
  char pubkey[SIG_BUF_SIZE];
  uint64_t len;
} script_sig;


uint32_t btc_uint4(FILE *fp);
void get_hex2_str(char *buf, const uint8_t b);
uint64_t btc_varint(FILE *fp);
void btc_hash(char *tx_hash_str, FILE *fp);
void btc_sc_sig(script_sig *sig_ptr, FILE *fp);
void cp_sig_hex_to_str(char *sig_str, const uint8_t sig_buf[], const uint64_t len);
void read_op_pushdata0(char *str, uint8_t op_code, FILE *fp);
void btc_cb_sig(coinbase_sig *sig_ptr, FILE *fp);

int main(int argc, char *argv[])
{
  FILE *fp;
  uint32_t tx_version;
  uint64_t tx_vin;
  uint32_t pre_txout_inx;
  char pre_tx_hash_str[65];
  uint64_t txin_script_len;
  char *txin_sig;
  uint32_t txin_seqno;
  script_sig sc_sig;
  coinbase_sig cb_sig;

  sc_sig.len = 0;
  cb_sig.len = 0;
  
  fp = fopen(argv[1], "rb");
  tx_version = btc_uint4(fp);
  tx_vin = btc_varint(fp);
  btc_hash(pre_tx_hash_str, fp);
  pre_txout_inx = btc_uint4(fp);
  txin_script_len = btc_varint(fp);
  if(pre_txout_inx == COINBASE_INX && strcmp(pre_tx_hash_str, NULL_HASH) == 0){
    cb_sig.len = txin_script_len;
    btc_cb_sig(&cb_sig, fp);
  } else {
    sc_sig.len = txin_script_len;
    btc_sc_sig(&sc_sig, fp);
  }

  txin_seqno = btc_uint4(fp);

  printf("Tx Version: %u\n", tx_version);
  printf("Tx In-counter: %llu\n", tx_vin);
  printf("Txin Previous Tx Hash: %s\n", pre_tx_hash_str);
  printf("Txin Previous Txout-index: %x\n", pre_txout_inx);
  printf("Txin-script length: %llu\n", txin_script_len);
  if(cb_sig.len > 0){
    printf("Txin-script / coinbase: %s\n", cb_sig.sig);
  }
  if(sc_sig.len > 0){
    printf("Txin-script / scriptSig: %s %s\n", sc_sig.sig, sc_sig.pubkey);
  }
  printf("Txin sequence_no: %x\n", txin_seqno);
  

  fclose(fp);
  
}


void get_hex2_str(char *buf, const uint8_t b)
{
  snprintf(buf, 3, "%02x", b);
}

uint32_t btc_uint4(FILE *fp)
{
  uint32_t buf;
  fread(&buf, sizeof(buf), 1, fp);

  return buf;
}

uint64_t btc_varint(FILE *fp)
{
  uint8_t  tx_vin1;
  uint16_t tx_vin2;
  uint32_t tx_vin4;
  uint64_t tx_vin8;
  uint64_t tx_vin;

  fread(&tx_vin1, sizeof(tx_vin1), 1, fp);
  if(tx_vin1 < 0xFD){
    tx_vin = tx_vin1;
  } else if(tx_vin1 == 0xFD){
    fread(&tx_vin2, sizeof(tx_vin2), 1, fp);
    tx_vin = tx_vin2;
  } else if(tx_vin1 == 0xFE) {
    fread(&tx_vin4, sizeof(tx_vin4), 1, fp);
    tx_vin = tx_vin4;
  } else if(tx_vin1 == 0xFF) {
    fread(&tx_vin8, sizeof(tx_vin8), 1, fp);
    tx_vin = tx_vin8;
  } else {
    tx_vin = 0;
  }

  return tx_vin;
}

void btc_hash(char *tx_hash_str, FILE *fp)
{
  uint8_t tx_hash[32];
  char buf3[3];

  fread(tx_hash, sizeof(uint8_t), 32, fp);
  for(int i=31; i >=0; --i)
  {
    get_hex2_str(buf3, tx_hash[i]);
    tx_hash_str[(31-i) *2] = buf3[0];
    tx_hash_str[(31-i) *2 + 1] = buf3[1];
    if(i == 0){
      tx_hash_str[64] = '\0';
    }
  }
}

void btc_cb_sig(coinbase_sig *sig_ptr, FILE *fp)
{
  uint64_t len = sig_ptr -> len;
  uint8_t sig_buf[len];
  char buf3[3];
  
  fread(sig_buf, sizeof(uint8_t), len, fp);
  for(uint64_t i=0; i < len; i++)
  {
    get_hex2_str(buf3, sig_buf[i]);
    sig_ptr->sig[i * 2] = buf3[0];
    sig_ptr->sig[i * 2 + 1] = buf3[1];
  }

  sig_ptr->sig[len * 2] = '\0';

}

void btc_sc_sig(script_sig *sig_ptr, FILE *fp)
{
  uint8_t op_code;
  char buf3[3];

  fread(&op_code, sizeof(uint8_t), 1, fp);
  if(op_code >= OP_PUSHDATA0_START && op_code <= OP_PUSHDATA0_END){
    read_op_pushdata0(sig_ptr -> sig, op_code, fp);
  } else {
  }

  fread(&op_code, sizeof(uint8_t), 1, fp);
  if(op_code >= OP_PUSHDATA0_START && op_code <= OP_PUSHDATA0_END){
    read_op_pushdata0(sig_ptr -> pubkey, op_code, fp);
  } else {
  }

}


void read_op_pushdata0(char *str, uint8_t op_code, FILE *fp)
{
  uint8_t *sig_buf;
  
  sig_buf = malloc(op_code * sizeof(uint8_t));
  fread(sig_buf, sizeof(uint8_t), op_code, fp);
  cp_sig_hex_to_str(str, sig_buf, op_code);

  free(sig_buf);
}


void cp_sig_hex_to_str(char *sig_str, const uint8_t sig_buf[], uint64_t len)
{
  char buf3[3];
  for(int64_t i=0; i < len; i++)
  {
    get_hex2_str(buf3, sig_buf[i]);
    sig_str[i * 2] = buf3[0];
    sig_str[i * 2 + 1] = buf3[1];
  }

  sig_str[len * 2] = '\0';
}

