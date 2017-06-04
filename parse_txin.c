#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t btc_uint4(FILE *fp);
void get_hex2_str(char *buf, const uint8_t b);
uint64_t btc_varint(FILE *fp);
void btc_hash(char *tx_hash_str, FILE *fp);
void btc_sig(char *sig_str, uint64_t len, FILE *fp);

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

  fp = fopen(argv[1], "rb");
  tx_version = btc_uint4(fp);
  tx_vin = btc_varint(fp);
  btc_hash(pre_tx_hash_str, fp);
  pre_txout_inx = btc_uint4(fp);
  txin_script_len = btc_varint(fp);
  txin_sig = malloc(2*txin_script_len * sizeof(uint8_t) + 1);
  btc_sig(txin_sig, txin_script_len, fp);
  txin_seqno = btc_uint4(fp);

  printf("Tx Version: %u\n", tx_version);
  printf("Tx In-counter: %llu\n", tx_vin);
  printf("Txin Previous Tx Hash: %s\n", pre_tx_hash_str);
  printf("Txin Previous Txout-index: %x\n", pre_txout_inx);
  printf("Txin-script length: %llu\n", txin_script_len);
  printf("Txin-script / scriptSig: %s\n", txin_sig);
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

  size_t ret_code = fread(tx_hash, sizeof(uint8_t), 32, fp);
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

void btc_sig(char *sig_str, uint64_t len, FILE *fp)
{
  uint8_t sig_buf[len];
  char buf3[3];
  
  size_t ret_code = fread(sig_buf, sizeof(uint8_t), len, fp);
  for(int64_t i=0; i < len; i++)
  {
    get_hex2_str(buf3, sig_buf[i]);
    sig_str[i * 2] = buf3[0];
    sig_str[i * 2 + 1] = buf3[1];
  }

  sig_str[len * 2 + 1] = '\0';
  
}

