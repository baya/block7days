#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "beej_pack.h"


#define NT_MAGIC_MAIN  0xD9B4BEF9
#define NT_MAGIC_TEST  0xDAB5BFFA
#define NT_MAGIC_TEST3 0x0709110B
#define NT_MAGIC_NC    0xFEB4BEF9

#define LOCAL_IP_SRC "::ffff:127.0.0.1"
#define PL_BUF_SIZE 1024

static const uint8_t  V_PAD1 = 0xfd;
static const uint8_t  V_PAD2 = 0xfe;
static const uint8_t  V_PAD3 = 0xff;

static const uint8_t  N1 = 0xff;
static const uint16_t N2 = 0xffff;
static const uint32_t N4 = 0xffffffff;
static const uint64_t N8 = 0xffffffffffffffff;

typedef struct varint {
    uint8_t  va1;
    uint16_t va2;
    uint32_t va4;
    uint64_t va8;
    uint64_t value;
} varint;

typedef struct var_length_string{
    varint len;
    char *str;
} var_str;

typedef struct protocol_message_payload {
    uint32_t len;
    unsigned char buf[PL_BUF_SIZE];
} ptl_payload;

typedef struct protocol_btc_message{
    uint32_t magic;
    char cmd[12];
    uint32_t len;
    uint32_t checksum;
    ptl_payload *pld_ptr;
} ptl_msg;

typedef struct protocol_btc_net_addr{
    uint64_t servs;
    unsigned char ipv[16];
    uint16_t port;
} ptl_net_addr;

typedef struct protocol_btc_version{
    int32_t vers;
    uint64_t servs;
    int64_t ttamp;
    ptl_net_addr *addr_recv_ptr;
    ptl_net_addr *addr_from_ptr;
    uint64_t nonce;
    varint ua_len;
    var_str uagent;
    int32_t start_height;
    uint8_t relay;
    uint32_t len;
} ptl_ver;


void encode_varint(varint *vt, uint64_t i);
void build_version_payload(ptl_ver * ver, ptl_payload *pld);
ptl_net_addr *build_net_addr();
void build_btc_message(ptl_msg * msg, const char *cmd, ptl_payload *pld);
void pack_version(ptl_ver *ver, ptl_payload *pld);
static unsigned int pack_ptl_net_addr(ptl_net_addr *na, ptl_payload *pld);

int main(void)
{
    ptl_msg msg;
    ptl_ver ver;
    ptl_payload pld;
    uint32_t i;

    build_version_payload(&ver, &pld);
    build_btc_message(&msg, "version", &pld);

    printf("msg.cmd: %s\n", msg.cmd);
    printf("msg.len: %u\n", msg.len);
    printf("ver.vers: %u\n", ver.vers);
    printf("ver.addr_recv_ptr -> servs: %llu\n", ver.addr_recv_ptr -> servs);
    printf("ver.addr_recv_ptr -> port: %u\n", ver.addr_recv_ptr -> port);
    //printf("ver.addr_recv_ptr -> ipv: %02x\n", ver.addr_recv_ptr -> ipv[0]);
    printf("ver.ua_len: %llu\n", ver.ua_len.value);
    printf("pld.len: %u\n", pld.len);
    printf("pld.buf: ");
    for(i = 0; i < pld.len; i++){
	printf("%02x", pld.buf[i]);
    }
    printf("\n");
	
    
}

void build_btc_message(ptl_msg * msg, const char *cmd, ptl_payload *pld)
{
    msg -> magic = NT_MAGIC_MAIN;
    strcpy(msg -> cmd, cmd);
    msg -> len = pld -> len;
    msg -> pld_ptr = pld;
}

void build_version_payload(ptl_ver * ver, ptl_payload *pld)
{
    varint *variptr;
    
    ver -> vers = 70014;
    ver -> servs = 1;
    ver -> ttamp = (int64_t)time(NULL);
    ver -> addr_recv_ptr = build_net_addr();
    ver -> addr_from_ptr = build_net_addr();
    ver -> nonce = 0;
    variptr = &(ver -> ua_len);
    encode_varint(variptr, 0);
    ver -> start_height = 329167;
    ver -> relay = 0;
    pld -> len = 0;
    pack_version(ver, pld);
}

void pack_version(ptl_ver *ver, ptl_payload *pld)
{
    unsigned int size;
    unsigned char *bufp = pld -> buf;
    
    size = beej_pack(bufp, "l", ver -> vers);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "Q", ver -> servs);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "q", ver -> ttamp);
    pld -> len += size;
    bufp += size;

    size = pack_ptl_net_addr(ver -> addr_recv_ptr, pld);
    pld -> len += size;
    bufp += size;

    size = pack_ptl_net_addr(ver -> addr_from_ptr, pld);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "Q", ver -> nonce);
    pld -> len += size;
    bufp += size;

}

unsigned int pack_ptl_net_addr(ptl_net_addr *na, ptl_payload *pld)
{
    unsigned int size = 0;
    unsigned int m_size = 0;
    unsigned char *bufp = pld -> buf;

    bufp += pld -> len;
    size = beej_pack(bufp, "Q", na -> servs);
    m_size += size;
    bufp += size;

    size = 16;
    memcpy(bufp, na -> ipv, size);
    m_size += size;
    bufp += size;

    size = beej_pack(bufp, "H", na -> port);
    m_size += size;
    bufp += size;

    return m_size;
}

unsigned int pack_varint(variant v, ptl_payload *pld)
{
    unsigned int size = 0;
    unsigned int m_size = 0;
    unsigned char *bufp = pld -> buf;

    bufp += pld -> len;

}


ptl_net_addr *build_net_addr()
{
    ptl_net_addr *na_ptr;
    char *ip_src = "::ffff:127.0.0.1";
    int s, domain;
    
    domain = AF_INET6;

    na_ptr = malloc(sizeof *na_ptr);
    na_ptr -> servs = 1;
    na_ptr -> port = 8333;
    s = inet_pton(domain, LOCAL_IP_SRC, na_ptr -> ipv);
    if (s <= 0) {
	if (s == 0)
	    fprintf(stderr, "Not in presentation format");
	else
	    perror("inet_pton");
	exit(EXIT_FAILURE);
    }

    return na_ptr;
}


  
void encode_varint(varint *vt, uint64_t i)
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
