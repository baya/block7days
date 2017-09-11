#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "kyk_base58.h"
#include "kyk_script.h"
#include "kyk_utils.h"

static size_t build_p2pkh_sc_pubk(unsigned char *buf, const unsigned char *pkh, size_t pkh_len);
static int pubk_hash_from_address(unsigned char *pubk_hash, size_t pkh_len, const char *addr);
static int is_sc_na_const(uint8_t opcode);
static void init_sc_stack(struct kyk_sc_stack *stk);
static void kyk_sc_stack_push(struct kyk_sc_stack *stk, uint8_t *sc, size_t len);
static int is_sc_na_const(uint8_t opcode);
static void kyk_sc_op_dup(struct kyk_sc_stack *stk);

size_t p2pkh_sc_from_address(unsigned char *sc, const char *addr)
{
    uint8_t pubk_hash[RIPEMD160_DIGEST_LENGTH];
    size_t len;
    
    if(pubk_hash_from_address(pubk_hash, sizeof(pubk_hash), addr) < 0){
	return -1;
    }
    
    len = build_p2pkh_sc_pubk(sc, pubk_hash, sizeof(pubk_hash));

    return len;
}

int pubk_hash_from_address(unsigned char *pubk_hash, size_t pkh_len, const char *addr)
{
    BIGNUM bn;
    size_t len;
    uint8_t buf[1 + RIPEMD160_DIGEST_LENGTH + 4];
    uint8_t csum[4];

    BN_init(&bn);
    raw_decode_base58(&bn, addr, strlen(addr));

    len = BN_num_bytes(&bn);
    memset(buf, 0, sizeof(buf));
    BN_bn2bin(&bn, buf + sizeof(buf) - len);

    BN_free(&bn);

    if(validate_base58_checksum(buf, 1 + RIPEMD160_DIGEST_LENGTH) < 0){
	fprintf(stderr, "address base58 checksum failed\n");
	return -1;
    }

    memcpy(pubk_hash, buf+1, pkh_len);

    return 1;

}

/* 76       A9             14                                                        */
/* OP_DUP OP_HASH160    Bytes to push                                                */
/* 89 AB CD EF AB BA AB BA AB BA AB BA AB BA AB BA AB BA AB BA   88         AC       */
/*                       Data to push                     OP_EQUALVERIFY OP_CHECKSIG */
size_t build_p2pkh_sc_pubk(unsigned char *sc, const unsigned char *pkh, size_t pkh_len)
{
    size_t count = 0;
    
    *sc = OP_DUP;
    count += 1;

    sc++;
    *sc = OP_HASH160;
    count += 1;

    sc++;
    *sc = (uint8_t) pkh_len;
    count += 1;

    sc++;
    memcpy(sc, pkh, pkh_len);
    count += pkh_len;
    
    sc += pkh_len;
    *sc = OP_EQUALVERIFY;
    count += 1;

    sc++;
    *sc = OP_CHECKSIG;
    count += 1;

    return count;
}


size_t kyk_combine_sc(uint8_t *sc,
		    uint8_t *sc_sig, size_t sc_sig_len,
		    uint8_t *sc_pubk, size_t sc_pubk_len)
{
    size_t count = 0;
    
    for(int i=0; i < sc_sig_len; i++){
	*sc = sc_sig[i];
	sc++;
	count++;
    }

    for(int i=0; i < sc_pubk_len; i++){
	*sc = sc_pubk[i];
	sc++;
	count++;
    }

    return count;
}

int kyk_run_sc(uint8_t *sc, size_t sc_len)
{
    struct kyk_sc_stack stk;
    uint8_t opcode;
    size_t count = 0;
    int ret_code = 1;

    init_sc_stack(&stk);

    while(count < sc_len){
	opcode = *sc;
	if(is_sc_na_const(opcode) == 1){
	    sc++;
	    count += 1;
	    kyk_sc_stack_push(&stk, sc, opcode);
	    sc += opcode;
	    count += opcode;
	} else {
	    switch (opcode){
	    case OP_DUP:
		sc++;
		count += 1;
		kyk_sc_op_dup(&stk);
		break;
	    case OP_HASH160:
		sc++;
		count += 1;
		kyk_sc_op_hash160(&stk);
	    default:		
	        fprintf(stderr, "Invalid Op Code: %d\n", opcode);
		ret_code = 0;
		break;
	    }
	}
    }

    

    return ret_code;
    
}

void init_sc_stack(struct kyk_sc_stack *stk)
{
    stk -> op_start = stk -> op_end = stk -> buf;
}

void kyk_sc_op_hash160(struct kyk_sc_stack *stk)
{
}

void kyk_sc_op_dup(struct kyk_sc_stack *stk)
{
    uint8_t *dup = stk -> op_start;
    uint8_t *dup_end = stk -> op_end;
    
    while(dup < stk -> op_end){
	*dup_end = *dup;
	dup_end++;
	dup++;
    }

    stk -> op_start = dup;
    stk -> op_end = dup_end;

}


void kyk_sc_stack_push(struct kyk_sc_stack *stk, uint8_t *sc, size_t len)
{
    stk -> op_start = stk -> op_end;
    memcpy(stk -> op_start, sc, len);
    stk -> op_end += len;
}

int is_sc_na_const(uint8_t opcode)
{
  if(opcode >= OP_PUSHDATA0_START && opcode <= OP_PUSHDATA0_END){
    return 1;
  } else {
    return 0;
  }
}




