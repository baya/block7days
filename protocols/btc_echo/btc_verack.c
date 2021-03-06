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

int main(void)
{
    ptl_msg msg;
    ptl_payload pld;
    ptl_msg *resp_msg;
    ptl_msg_buf msg_buf;
    ptl_resp_buf resp_buf;
    ptl_msg_buf resp_msg_buf;

    msg.len = 0;
    pld.len = 0;
    build_btc_message(&msg, "verack", &pld);
    pack_btc_message(&msg_buf, &msg);

    kyk_send_btc_msg_buf("localhost", "8333", &msg_buf, &resp_buf);
    
    printf("=======> Response Body:\n");
    printf("%s\n", resp_buf.body);
}
