#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define NT_MAGIC_MAIN  0xD9B4BEF9
#define NT_MAGIC_TEST  0xDAB5BFFA
#define NT_MAGIC_TEST3 0x0709110B
#define NT_MAGIC_NC    0xFEB4BEF9

static const uint8_t  V_PAD1 = 0xfd;
static const uint8_t  V_PAD2 = 0xfe;
static const uint8_t  V_PAD3 = 0xff;

static const uint8_t  N1 = 0xff;
static const uint16_t N2 = 0xffff;
static const uint32_t N4 = 0xffffffff;
static const uint64_t N8 = 0xffffffffffffffff;

void encode_btc_varint(btc_varint *vt, uint64_t i);

typedef struct var_int {
  uint8_t  va1;
  uint16_t va2;
  uint32_t va4;
  uint64_t va8;
  uint64_t value;
} var_int;

typedef struct var_length_string{
  var_int len;
  char *str;
} var_str;

typedef struct protocol_btc_message{
  uint32_t magic;
  char cmd[12];
  uint32_t len;
  uint32_t checksum;
  unsigned char *payload;
} ptl_msg;

typedef struct protocol_btc_net_addr{
  uint32_t time;
  uint64_t servs;
  char ipv[16];
  uint16_t port;
} ptl_net_addr;

typedef struct protocol_btc_version{
  int32_t vers;
  uint64_t servs;
  int64_t ttamp;
  ptl_net_addr *addr_recv;
  ptl_net_addr *addr_from;
  uint64_t nonce;
  var_str uagent;
  int32_t start_height;
  uint8_t relay;
} ptl_ver;

int main(void)
{
  ptl_msg msg;

  msg.magic = NT_MAGIC_MAIN;
  msg.cmd = "version";
  msg.len = 0;
  
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
