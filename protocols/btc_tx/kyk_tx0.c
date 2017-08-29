#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "kyk_tx.h"

int hexstr_to_bytes(const char *hexstr, unsigned char *buf, size_t len);

int main()
{
    struct kyk_tx tx0;
    struct kyk_txin txin;
    tx0.version = 1;
    tx0.vin_sz = 1;
    char *hstr = "0000000000000000000000000000000000000000000000000000000000000000";
    // char *hstr = "0437cd7f8525ceed2324359c2d0ba26006d92d856a9c20fa0241106ee5a597c9";
    if(hexstr_to_bytes(hstr, txin.pre_txid, 32) == -1){
	fprintf(stderr, "failed in function hexstr_to_bytes()\n");
	exit(1);
    };

    for(int i=0; i < 32; i++){
	printf("%02x", txin.pre_txid[i]);
    }
    printf("\n");
}
    
int hexstr_to_bytes(const char *hexstr, unsigned char *buf, size_t len)
{
    size_t count = 0;
    size_t dst_len = len * 2;
    int ret;

    // assert(strlen(hexstr) == dst_len);

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
