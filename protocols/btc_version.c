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
#include <openssl/sha.h>

#include "btc_message.h"
#include "kyk_sha.h"
#include "beej_pack.h"
#include "kyk_socket.h"

static const uint8_t  V_PAD1 = 0xfd;
static const uint8_t  V_PAD2 = 0xfe;
static const uint8_t  V_PAD3 = 0xff;

static const uint8_t  N1 = 0xff;
static const uint16_t N2 = 0xffff;
static const uint32_t N4 = 0xffffffff;
static const uint64_t N8 = 0xffffffffffffffff;


void encode_varint(varint *vt, uint64_t i);
void build_version_payload(ptl_ver * ver, ptl_payload *pld);
ptl_net_addr *build_net_addr();
void build_btc_message(ptl_msg * msg, const char *cmd, ptl_payload *pld);
void pack_version(ptl_ver *ver, ptl_payload *pld);
unsigned int pack_ptl_net_addr(unsigned char *bufp, ptl_net_addr *na);
unsigned int pack_varint(unsigned char *bufp, varint v);
void encode_varstr(var_str *vstr, const char *src);
unsigned int pack_varstr(unsigned char *bufp, var_str vstr);
void print_version_payload(const ptl_payload *pld);
size_t print_hex(const unsigned char *buf, size_t len, int width, char *note);
void read_varint(const unsigned char *buf, varint *vt);
void pack_btc_message(ptl_msg_buf *msg_buf, ptl_msg *msg);
void print_msg_buf(const ptl_msg_buf *msg_buf);
void byte_swap(unsigned char* data, int len);

int main(void)
{
    ptl_msg msg;
    ptl_msg_buf msg_buf;
    ptl_ver ver;
    ptl_payload pld;
    uint32_t i;

    build_version_payload(&ver, &pld);
    build_btc_message(&msg, "version", &pld);
    pack_btc_message(&msg_buf, &msg);

    //kyk_send_btc_msg_buf("seed.bitcoin.sipa.be", "8333", &msg_buf);

    printf("msg.cmd: %s\n", msg.cmd);
    printf("msg.len: %u\n", msg.len);
    printf("ver.vers: %u\n", ver.vers);
    printf("ver.addr_recv_ptr -> servs: %llu\n", ver.addr_recv_ptr -> servs);
    printf("ver.addr_recv_ptr -> port: %u\n", ver.addr_recv_ptr -> port);
    printf("ver.ua_len: %llu\n", ver.ua_len.value);
    printf("pld.len: %u\n", pld.len);
    printf("pld.buf: ");
    for(i = 0; i < pld.len; i++){
	printf("%02x", pld.buf[i]);
    }
    printf("\n");

    print_msg_buf(&msg_buf);
    print_version_payload(&pld);
	
}

void print_msg_buf(const ptl_msg_buf *msg_buf)
{
    const unsigned char *buf = msg_buf -> body;
    size_t len = 0;
    int wth = 36;

    len = print_hex(buf, sizeof(uint32_t), wth, "Start String: Mainnet");
    buf += len;
    len = print_hex(buf, 12, wth, "Command name");
    buf += len;
    len = print_hex(buf, sizeof(uint32_t), wth, "Payload size");
    buf += len;

    len = print_hex(buf, 4, wth, "Checksum");
    buf += len;

    len = msg_buf -> len - 24;
    len = print_hex(buf, len, wth, "Payload");
    buf += len;

    for(int i=0; i < msg_buf -> len; i++){
	printf("%02x", msg_buf -> body[i]);
    }

    printf("\n");
}

void print_version_payload(const ptl_payload *pld)
{
    const unsigned char *buf = pld -> buf;
    varint vt;
    size_t len = 0;
    int wth = 36;
    
    len = print_hex(buf, sizeof(int32_t), wth, "ptl_ver.vers");
    buf += len;
    len = print_hex(buf, sizeof(uint64_t), wth, "ptl_ver.servs");
    buf += len;
    len = print_hex(buf, sizeof(int64_t), wth, "ptl_ver.ttamp");
    buf += len;
    len = print_hex(buf, sizeof(uint64_t), wth, "ptl_ver.addr_recv_ptr -> servs");
    buf += len;
    len = print_hex(buf, sizeof(unsigned char) * 16, wth, "ptl_ver.addr_recv_ptr -> ipv");
    buf += len;
    len = print_hex(buf, sizeof(uint16_t), wth, "ptl_ver.addr_recv_ptr -> port");
    buf += len;
    len = print_hex(buf, sizeof(uint64_t), wth, "ptl_ver.addr_from_ptr -> servs");
    buf += len;
    len = print_hex(buf, sizeof(unsigned char) * 16, wth, "ptl_ver.addr_from_ptr -> ipv");
    buf += len;
    len = print_hex(buf, sizeof(uint16_t), wth, "ptl_ver.addr_from_ptr -> port");
    buf += len;
    len = print_hex(buf, sizeof(uint64_t), wth, "ptl_ver.nonce");
    buf += len;
    read_varint(buf, &vt);
    len = print_hex(buf, sizeof(uint16_t), wth, "ptl_ver.ua_len");
    buf += len;
    len = print_hex(buf, vt.value, wth, "ptl_ver.uagent");
    buf += len;
    len = print_hex(buf, sizeof(int32_t), wth, "ptl_ver.start_height");
    buf += len;
    len = print_hex(buf, sizeof(uint8_t), wth, "ptl_ver.relay");
}

void read_varint(const unsigned char *buf, varint *vt)
{
    uint8_t va1 = buf[0];
    unsigned char *bbuf = (unsigned char *) buf;
    
    vt -> va1 = va1;
    vt -> va2 = vt -> va4 = vt -> va8 = 0;
    vt -> value = 0;
    vt -> len = 0;
    
    if(va1 < 0xFD){
	vt -> value = va1;
	vt -> len = sizeof(uint8_t);
    } else if(va1 == 0xFD){
	beej_unpack(bbuf++, "<H", &(vt->va2));
	vt -> value = vt -> va2;
	vt -> len = sizeof(uint16_t);
    } else if(va1 == 0xFE) {
	beej_unpack(bbuf++, "<L", &(vt->va4));
	vt -> value = vt -> va4;
	vt -> len = sizeof(uint32_t);
    } else if(va1 == 0xFF) {
	beej_unpack(bbuf++, "<Q", &(vt->va8));
	vt -> value = vt -> va8;
	vt -> len = sizeof(uint64_t);
    } else {
    }
}

size_t print_hex(const unsigned char *buf, size_t len, int width, char *note)
{
    for(int i=0; i < len; i++){
	printf("%02x", *buf++);
    }

    printf(" ");
    for(int j=len*2; j < width; j++){
	printf(".");
    }

    printf(" %s\n", note);

    return len;
}

void build_btc_message(ptl_msg * msg, const char *cmd, ptl_payload *pld)
{
    unsigned char *dg2;
    
    msg -> magic = NT_MAGIC_MAIN;
    strcpy(msg -> cmd, cmd);
    msg -> len = pld -> len;
    msg -> pld_ptr = pld;
    dg2 = kyk_dble_sha256((char *)pld -> buf);
    //byte_swap(dg2, SHA256_DIGEST_LENGTH);
    memcpy(msg -> checksum, dg2, 4);
}

void pack_btc_message(ptl_msg_buf *msg_buf, ptl_msg *msg)
{
    unsigned char *buf = msg_buf -> body;
    size_t size=0;
    
    msg_buf -> len = 0;
    size = beej_pack(buf, "<L", NT_MAGIC_MAIN);
    msg_buf -> len += size;
    buf += size;

    size = sizeof(msg -> cmd);
    memcpy(buf, msg -> cmd, size);
    msg_buf -> len += size;
    buf += size;
    
    size = beej_pack(buf, "<L", (msg -> pld_ptr) -> len);
    msg_buf -> len += size;
    buf += size;

    size = sizeof(msg -> checksum);
    memcpy(buf, msg -> checksum, size);
    msg_buf -> len += size;
    buf += size;

    size = msg -> pld_ptr -> len;
    memcpy(buf, msg -> pld_ptr -> buf, size);
    msg_buf -> len += size;
    buf += size;
    
}


void build_version_payload(ptl_ver * ver, ptl_payload *pld)
{
    //ver -> vers = 70014;
    //ver -> vers = 31900;
    ver -> vers = 70014;
    ver -> servs = NODE_NETWORK;
    //ver -> ttamp = (int64_t)time(NULL);
    //ver -> ttamp = 0x73bc8659;
    ver -> ttamp = 0x5986bc73;
    ver -> addr_recv_ptr = build_net_addr();
    ver -> addr_from_ptr = build_net_addr();
    ver -> nonce = 0;
    //encode_varstr(&(ver -> uagent), "/Satoshi:0.9.2.1/");
    encode_varstr(&(ver -> uagent), "");
    ver -> ua_len = ver -> uagent.len;
    ver -> start_height = 329167;
    //ver -> start_height = 98645;
    ver -> relay = 0;
    pld -> len = 0;
    pack_version(ver, pld);
}

void encode_varstr(var_str *vstr, const char *src)
{
    size_t len;

    len = strlen(src);
    encode_varint(&(vstr -> len), len);
    vstr -> body = malloc(len * sizeof(char));
    memcpy(vstr -> body, src, len * sizeof(char));
}

void pack_version(ptl_ver *ver, ptl_payload *pld)
{
    unsigned int size;
    unsigned char *bufp = pld -> buf;
    
    size = beej_pack(bufp, "<l", ver -> vers);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "<Q", ver -> servs);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "<q", ver -> ttamp);
    pld -> len += size;
    bufp += size;

    size = pack_ptl_net_addr(bufp, ver -> addr_recv_ptr);
    pld -> len += size;
    bufp += size;

    size = pack_ptl_net_addr(bufp, ver -> addr_from_ptr);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "<Q", ver -> nonce);
    pld -> len += size;
    bufp += size;

    size = pack_varint(bufp, ver -> ua_len);
    size = beej_pack(bufp, "<H", 0);
    pld -> len += size;
    bufp += size;

    size = pack_varstr(bufp, ver -> uagent);
    pld -> len += size;
    bufp += size;

    size = beej_pack(bufp, "<l", ver -> start_height);
    pld -> len += size;
    bufp += size;

    /* size = beej_pack(bufp, "C", ver -> relay); */
    /* pld -> len += size; */
    /* bufp += size; */

}

unsigned int pack_ptl_net_addr(unsigned char *bufp, ptl_net_addr *na)
{
    unsigned int size = 0;
    unsigned int m_size = 0;

    size = beej_pack(bufp, "<Q", na -> servs);
    m_size += size;
    bufp += size;

    size = 16;
    memcpy(bufp, na -> ipv, size);
    m_size += size;
    bufp += size;

    size = beej_pack(bufp, ">H", na -> port);
    m_size += size;
    bufp += size;

    return m_size;
}

unsigned int pack_varstr(unsigned char *bufp, var_str vstr)
{
    unsigned int size = 0;
    unsigned int m_size = 0;

    if(vstr.len.value > 0)
    {
	size = vstr.len.value * sizeof(char);
	memcpy(bufp, vstr.body, size);
	m_size += size;

    }

    return m_size;

}

unsigned int pack_varint(unsigned char *bufp, varint v)
{
    unsigned int size = 0;
    unsigned int m_size = 0;

    size = beej_pack(bufp, "C", v.va1);
    m_size += size;
    bufp += size;
    
    if(v.value == 0){
    } else {
	if(v.va2 > 0){
	    size = beej_pack(bufp, "H", v.va2);
	    m_size += size;
	}
	if(v.va4 > 0){
	    size = beej_pack(bufp, "L", v.va4);
	    m_size += size;
	}
	if(v.va8 > 0){
	    size = beej_pack(bufp, "Q", v.va8);
	    m_size += size;
	}
    }

    return m_size;
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
    vt->len = 0;

    vt->len = sizeof(uint8_t);
    if(i < V_PAD1){
	vt->va1 = i;	
    } else if(i >= V_PAD1 && i <= N2){
	vt->va1 = V_PAD1;
	vt->va2 = i;
	vt->len += sizeof(uint16_t);
    } else if(i >= V_PAD2 && i <= N4){
	vt->va1 = V_PAD2;
	vt->va4 = i;
	vt->len += sizeof(uint32_t);
    } else if(i >= V_PAD3 && i <= N8){
	vt->va1 = V_PAD3;
	vt->va8 = i;
	vt->len += sizeof(uint64_t);
    }

    vt->value = i;
}


void byte_swap(unsigned char* data, int len)
{
    int c;
    unsigned char tmp[len];
       
    c=0;
    while(c<len)
    {
	tmp[c] = data[len-(c+1)];
	c++;
    }
       
    c=0;
    while(c<len)
    {
	data[c] = tmp[c];
	c++;
    }
}
