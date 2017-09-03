#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>


#include "kyk_tx.h"
#include "kyk_sha.h"

#define MAX_BUF_SIZE 10000

int hexstr_to_bytes(const char *hexstr, unsigned char *buf, size_t len);

struct kyk_txin *create_txin(const char *pre_txid,
			     uint32_t pre_tx_inx,
			     varint_t sc_size,
			     const char *sc,
			     uint32_t seq_no);

struct kyk_txout *create_txout(uint64_t value,
			       varint_t sc_size,
			       const char *sc);

void print_bytes_in_hex(const unsigned char *buf, size_t len);


int main()
{
    unsigned char buf[MAX_BUF_SIZE];
    size_t count;
    struct kyk_tx tx0;
    char *pre_txid = "0000000000000000000000000000000000000000000000000000000000000000";
    char *txin_sc = "04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73";
    char *txout_sc = "4104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac";
    struct kyk_hash *txid;
    
    tx0.version = 1;
    tx0.vin_sz = 1;
    tx0.lock_time = 0;

    tx0.txin = create_txin(pre_txid,
		       4294967295,
		       77,
		       txin_sc,
		       0xFFFFFFFF);

    tx0.vout_sz = 1;
    tx0.txout = create_txout(5000000000,
			     67,
			     txout_sc);

    count = kyk_seri_tx(buf, &tx0);
    printf("====>hex: ");
    print_bytes_in_hex(buf, count);
    
    txid = kyk_inver_hash((char *)buf, count);
    printf("====>hash: ");
    print_bytes_in_hex(txid -> body, txid -> len);
    printf("\n");

}

void print_bytes_in_hex(const unsigned char *buf, size_t len)
{
    for(int i=0; i < len; i++){
	printf("%02x", buf[i]);
    }
    printf("\n");
}
    
int hexstr_to_bytes(const char *hexstr, unsigned char *buf, size_t len)
{
    size_t count = 0;
    size_t dst_len = len * 2;
    int ret;

    if(strlen(hexstr) != dst_len){
	return -1;
    }

    for(count = 0; count < len; count++){
	ret = sscanf(hexstr, "%2hhx", buf);
	if(ret < 1){
	    return -1;
	}
	buf += 1;
	hexstr += 2;
    }

    return 0;
}

struct kyk_txin *create_txin(const char *pre_txid,
			     uint32_t pre_tx_inx,
			     varint_t sc_size,
			     const char *sc,
			     uint32_t seq_no)
{
    struct kyk_txin *txin = malloc(sizeof(struct kyk_txin));
    if(txin == NULL){
	fprintf(stderr, "failed in malloc kyk_txin \n");
	exit(1);
    }

    if(hexstr_to_bytes(pre_txid, txin->pre_txid, 32) == -1){
	fprintf(stderr, "failed in setting pre_txid \n");
	exit(1);
    }

    txin->pre_tx_inx = pre_tx_inx;
    txin->sc_size = sc_size;
    txin->sc = malloc(sc_size * sizeof(unsigned char));
    if(hexstr_to_bytes(sc, txin->sc, sc_size) == -1){
	fprintf(stderr, "failed in setting txin sc \n");
	exit(1);
    }

    txin->seq_no = seq_no;

    return txin;
}

struct kyk_txout *create_txout(uint64_t value,
			       varint_t sc_size,
			       const char *sc)
{
    struct kyk_txout *txout = malloc(sizeof(struct kyk_txout));
    if(txout == NULL){
	fprintf(stderr, "failed in malloc kyk_txout \n");
	exit(1);
    }

    txout -> value = value;
    txout -> sc_size = sc_size;
    txout -> sc = malloc(sc_size * sizeof(unsigned char));
    
    if(hexstr_to_bytes(sc, txout->sc, sc_size) == -1){
	fprintf(stderr, "failed in setting txout sc \n");
	exit(1);
    }

    return txout;
}


