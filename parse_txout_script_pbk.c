#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define COINBASE_INX       0xffffffff
#define NULL_HASH          "0000000000000000000000000000000000000000000000000000000000000000"

/* Constants */
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

/* Flow control */
#define OP_NOP    0x61
#define OP_IF     0x63
#define OP_NOTIF  0x64
#define OP_ELSE   0x67
#define OP_ENDIF  0x68
#define OP_VERIFY 0x69
#define OP_RETURN 0x6a

/* Stack */
#define OP_TOALTSTACK   0x6b
#define OP_FROMALTSTACK 0x6c
#define OP_IFDUP        0x73
#define OP_DEPTH        0x74
#define OP_DROP         0x75
#define OP_DUP          0x76
#define OP_NIP          0x77
#define OP_OVER         0x78
#define OP_PICK         0x79
#define OP_ROLL         0x7a
#define OP_ROT          0x7b
#define OP_SWAP         0x7c
#define OP_TUCK         0x7d
#define OP_2DROP        0x6d
#define OP_2DUP         0x6e
#define OP_3DUP         0x6f
#define OP_2OVER        0x70
#define OP_2ROT         0x71
#define OP_2SWAP        0x72

/* Splice */
#define OP_CAT    0x7e
#define OP_SUBSTR 0x7f
#define OP_LEFT   0x80
#define OP_RIGHT  0x81
#define OP_SIZE   0x82

/* Bitwise logic */
#define OP_INVERT      0x83
#define OP_AND         0x84
#define OP_OR          0x85
#define OP_XOR         0x86
#define OP_EQUAL       0x87
#define OP_EQUALVERIFY 0x88

/* Arithmetic */
#define OP_1ADD               0x8b
#define OP_1SUB               0x8c
#define OP_2MUL               0x8d
#define OP_2DIV               0x8e
#define OP_NEGATE             0x8f
#define OP_ABS                0x90
#define OP_NOT                0x91
#define OP_0NOTEQUAL          0x92
#define OP_ADD                0x93
#define OP_SUB                0x94
#define OP_MUL                0x95
#define OP_DIV                0x96
#define OP_MOD                0x97
#define OP_LSHIFT             0x98
#define OP_RSHIFT             0x99
#define OP_BOOLAND            0x9a
#define OP_BOOLOR             0x9b
#define OP_NUMEQUAL           0x9c
#define OP_NUMEQUALVERIFY     0x9d
#define OP_NUMNOTEQUAL        0x9e
#define OP_LESSTHAN           0x9f
#define OP_GREATERTHAN        0xa0
#define OP_LESSTHANOREQUAL    0xa1
#define OP_GREATERTHANOREQUAL 0xa2
#define OP_MIN                0xa3
#define OP_MAX                0xa4
#define OP_WITHIN             0xa5

/* Crypto */
#define OP_RIPEMD160           0xa6
#define OP_SHA1                0xa7
#define OP_SHA256              0xa8
#define OP_HASH160             0xa9
#define OP_HASH256             0xaa
#define OP_CODESEPARATOR       0xab
#define OP_CHECKSIG            0xac
#define OP_CHECKSIGVERIFY      0xad
#define OP_CHECKMULTISIG       0xae
#define OP_CHECKMULTISIGVERIFY 0xaf

/* Locktime */
#define OP_CHECKLOCKTIMEVERIFY 0xb1
#define OP_CHECKSEQUENCEVERIFY 0xb2

/* Pseudo-words */
#define OP_PUBKEYHASH    0xfd
#define OP_PUBKEY        0xfe
#define OP_INVALIDOPCODE 0xff

/* Reserved words */
#define OP_RESERVED  0x50
#define OP_VER       0x62
#define OP_VERIF     0x65
#define OP_VERNOTIF  0x66
#define OP_RESERVED1 0x89
#define OP_RESERVED2 0x8a
#define OP_NOP1      0xb0
#define OP_NOP9      0xb9

#define NO_FOUND_OPTCODE "NO_FOUND_OPTCODE"

#define SIG_BUF_SIZE 1000
#define PBK_STACK_SIZE 1000

typedef struct coinbase_sig {
  char sig[SIG_BUF_SIZE];
  uint64_t len;
} coinbase_sig;

typedef struct script_sig {
  char sig[SIG_BUF_SIZE];
  char pubkey[SIG_BUF_SIZE];
  uint64_t len;
} script_sig;

typedef struct script_pbk {
  char *stack[PBK_STACK_SIZE];
} script_pbk;

typedef struct tx_out {
  uint64_t value;
  uint64_t len;
  char *sc_pbk;
  script_pbk *parsed_sc_pbk;
} tx_out;



uint32_t btc_uint4(FILE *fp);
void get_hex2_str(char *buf, const uint8_t b);
uint64_t btc_varint(FILE *fp);
void btc_hash(char *tx_hash_str, FILE *fp);
void btc_sc_sig(script_sig *sig_ptr, FILE *fp);
void cp_sig_hex_to_str(char *sig_str, const uint8_t sig_buf[], const uint64_t len);
void read_op_pushdata0(char *str, uint8_t op_code, FILE *fp);
void btc_cb_sig(coinbase_sig *sig_ptr, FILE *fp);
tx_out *read_tx_out_list(uint64_t count, FILE *fp);
uint8_t btc_uint1(FILE *fp);
uint64_t btc_uint8(FILE *fp);
char *btc_sc_pbk(uint64_t len, FILE *fp);
script_pbk *btc_parsed_sc_pbk(uint64_t len, FILE *fp);
void print_parsed_sc_pbk(script_pbk *sc_pbk);
char *tr_opcode_to_name(uint8_t opcode);
int is_sc_na_constant(uint8_t opcode);

/* generated by parse_script_optcode_to_c_array.rb */
static char *btc_sc_opcode_names[] = {"OP_FALSE","OP_PUSHDATA0_START","OP_PUSHDATA0_END","OP_PUSHDATA1","OP_PUSHDATA2","OP_PUSHDATA4","OP_1NEGATE",
				      "OP_TRUE","OP_2","OP_3","OP_4","OP_5","OP_6","OP_7","OP_8","OP_9","OP_10","OP_11","OP_12","OP_13","OP_14","OP_15",
				      "OP_16","OP_NOP","OP_IF","OP_NOTIF","OP_ELSE","OP_ENDIF","OP_VERIFY","OP_RETURN","OP_TOALTSTACK","OP_FROMALTSTACK",
				      "OP_IFDUP","OP_DEPTH","OP_DROP","OP_DUP","OP_NIP","OP_OVER","OP_PICK","OP_ROLL","OP_ROT","OP_SWAP","OP_TUCK","OP_2DROP",
				      "OP_2DUP","OP_3DUP","OP_2OVER","OP_2ROT","OP_2SWAP","OP_CAT","OP_SUBSTR","OP_LEFT","OP_RIGHT","OP_SIZE","OP_INVERT",
				      "OP_AND","OP_OR","OP_XOR","OP_EQUAL","OP_EQUALVERIFY","OP_1ADD","OP_1SUB","OP_2MUL","OP_2DIV","OP_NEGATE","OP_ABS",
				      "OP_NOT","OP_0NOTEQUAL","OP_ADD","OP_SUB","OP_MUL","OP_DIV","OP_MOD","OP_LSHIFT","OP_RSHIFT","OP_BOOLAND","OP_BOOLOR",
				      "OP_NUMEQUAL","OP_NUMEQUALVERIFY","OP_NUMNOTEQUAL","OP_LESSTHAN","OP_GREATERTHAN","OP_LESSTHANOREQUAL","OP_GREATERTHANOREQUAL",
				      "OP_MIN","OP_MAX","OP_WITHIN","OP_RIPEMD160","OP_SHA1","OP_SHA256","OP_HASH160","OP_HASH256","OP_CODESEPARATOR","OP_CHECKSIG",
				      "OP_CHECKSIGVERIFY","OP_CHECKMULTISIG","OP_CHECKMULTISIGVERIFY","OP_CHECKLOCKTIMEVERIFY","OP_CHECKSEQUENCEVERIFY","OP_PUBKEYHASH",
				      "OP_PUBKEY","OP_INVALIDOPCODE","OP_RESERVED","OP_VER","OP_VERIF","OP_VERNOTIF","OP_RESERVED1","OP_RESERVED2","OP_NOP1","OP_NOP9"};

/* generated by parse_script_optcode_to_c_array.rb */
static uint8_t btc_sc_opcodes[] = {0x00,0x01,0x4b,0x4c,0x4d,0x4e,0x4f,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
				   0x60,0x61,0x63,0x64,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x6d,
				   0x6e,0x6f,0x70,0x71,0x72,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,
				   0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,
				   0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb1,0xb2,0xfd,0xfe,0xff,0x50,0x62,0x65,0x66,0x89,0x8a,0xb0,0xb9};

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
  uint64_t tx_vout;
  tx_out *txo_list;
  uint32_t lock_time;
  

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
  tx_vout = btc_varint(fp);

  txo_list = read_tx_out_list(tx_vout, fp);
  lock_time = btc_uint4(fp);

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

  printf("Txout Out-counter: %llu\n", tx_vout);
  for(int i=0; i < tx_vout; i++){
    printf("Txout Value: %llu\n", txo_list[i].value);
    printf("Txout-script length: %llu\n", txo_list[i].len);
    printf("Txout-script / scriptPubkey: %s\n", txo_list[i].sc_pbk);
    print_parsed_sc_pbk(txo_list[i].parsed_sc_pbk);
  }
  printf("Tx Lock Time: %x\n", lock_time);
  

  fclose(fp);
  
}


tx_out *read_tx_out_list(uint64_t count, FILE *fp)
{
  tx_out *txo_list;
  uint64_t tmp;

  txo_list = (tx_out*) malloc(count * sizeof(tx_out));
  for(uint64_t i=0; i < count; i++){
    txo_list[i].value = btc_uint8(fp);
    tmp = btc_varint(fp);
    txo_list[i].len = tmp;
    txo_list[i].sc_pbk = btc_sc_pbk(txo_list[i].len, fp);
    fseek(fp, -txo_list[i].len, SEEK_CUR);
    txo_list[i].parsed_sc_pbk = btc_parsed_sc_pbk(txo_list[i].len, fp);
  }

  return txo_list;
}

script_pbk *btc_parsed_sc_pbk(uint64_t len, FILE *fp)
{
  script_pbk *sc_pbk_ptr = malloc(sizeof(script_pbk));
  char **stack_ptr;
  uint64_t i = 0;
  uint8_t opcode;

  stack_ptr = sc_pbk_ptr -> stack;
  do {
    opcode = btc_uint1(fp);
    i += 1;
    if(is_sc_na_constant(opcode)){
      *stack_ptr = (char *)malloc((2 * opcode + 1) * sizeof(char));
      read_op_pushdata0(*stack_ptr, opcode, fp);
      i += opcode;
    } else {
      *stack_ptr = tr_opcode_to_name(opcode);
    }
    stack_ptr++;

  } while(i < len);


  return sc_pbk_ptr;
}

void print_parsed_sc_pbk(script_pbk *sc_pbk)
{
  char **stack_ptr = sc_pbk -> stack;
  printf("Txout-script / Parsed scriptPubkey: ");
  do{
    printf("%s ", *stack_ptr);
    stack_ptr++;
  } while(*stack_ptr);

  printf("\n");
}

int is_sc_na_constant(uint8_t opcode)
{
  if(opcode >= OP_PUSHDATA0_START && opcode <= OP_PUSHDATA0_END){
    return 1;
  } else {
    return 0;
  }
}

char *tr_opcode_to_name(uint8_t opcode)
{
  int i;
  char *res;
  size_t n = sizeof(btc_sc_opcodes) / sizeof(btc_sc_opcodes[0]);
  for(i = 0; i < n; i++)
  {
    if(btc_sc_opcodes[i] == opcode){
      break;
    }
  }

  if(i < n){
    res = btc_sc_opcode_names[i];
  } else {
    res = NO_FOUND_OPTCODE;
  }

  return res;
}

char *btc_sc_pbk(uint64_t len, FILE *fp)
{
  char *sc_pbk;
  uint8_t *buf;
  sc_pbk = (char *) malloc((2 * len + 1) * sizeof(char));
  buf = (uint8_t *) malloc(len * sizeof(uint8_t));
  fread(buf, sizeof(uint8_t), len, fp);
  cp_sig_hex_to_str(sc_pbk, buf, len);

  return sc_pbk;
}

uint64_t btc_uint8(FILE *fp)
{
  uint64_t buf;
  fread(&buf, sizeof(buf), 1, fp);
  return buf;
}

uint8_t btc_uint1(FILE *fp)
{
  uint8_t buf;
  fread(&buf, sizeof(buf), 1, fp);
  return buf;
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
  uint64_t len = 1;
  
  char buf3[3];

  fread(&op_code, sizeof(uint8_t), 1, fp);
  if(op_code >= OP_PUSHDATA0_START && op_code <= OP_PUSHDATA0_END){
    len += op_code;
    read_op_pushdata0(sig_ptr -> sig, op_code, fp);
  } else {
  }

  if(len < sig_ptr->len){
    fread(&op_code, sizeof(uint8_t), 1, fp);
    if(op_code >= OP_PUSHDATA0_START && op_code <= OP_PUSHDATA0_END){
      len += op_code;
      read_op_pushdata0(sig_ptr -> pubkey, op_code, fp);
    } else {
    }
  }

}


void read_op_pushdata0(char *str, uint8_t op_code, FILE *fp)
{
  uint8_t *sig_buf;
  sig_buf = (uint8_t *) malloc(op_code * sizeof(sig_buf));
  fread(sig_buf, sizeof(uint8_t), op_code, fp);
  cp_sig_hex_to_str(str, sig_buf, op_code);

  free(sig_buf);
}


void cp_sig_hex_to_str(char *sig_str, const uint8_t sig_buf[], uint64_t len)
{
  char buf3[3];
  for(uint64_t i=0; i < len; i++)
  {
    get_hex2_str(buf3, sig_buf[i]);
    sig_str[i * 2] = buf3[0];
    sig_str[i * 2 + 1] = buf3[1];
  }

  sig_str[len * 2] = '\0';
}

