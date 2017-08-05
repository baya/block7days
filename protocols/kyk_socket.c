#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "btc_message.h"


#define MAX_BUF_SIZE 1024

/* struct addrinfo { */
/*     int              ai_flags; */
/*     int              ai_family; */
/*     int              ai_socktype; */
/*     int              ai_protocol; */
/*     socklen_t        ai_addrlen; */
/*     struct sockaddr *ai_addr; */
/*     char            *ai_canonname; */
/*     struct addrinfo *ai_next; */
/* }; */

void kyk_send_btc_msg_buf(const char *node, const char *service, const ptl_msg_buf *msg_buf)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;    
    char resp_buf[MAX_BUF_SIZE];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* any type socket, SOCK_STREAM  or  SOCK_DGRAM */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(node, service, &hints, &result);
    if (s != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	exit(EXIT_FAILURE);
    }

/* getaddrinfo() returns a list of address structures.
   Try each address until we successfully connect(2).
   If socket(2) (or connect(2)) fails, we (close the socket
   and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
	sfd = socket(rp->ai_family,
		     rp->ai_socktype,
		     rp->ai_protocol);
	if (sfd == -1)
	    continue;

	if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1){
	    printf("connected success: %d\n", sfd);
	    break;                  /* Success */
	}

	close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
	fprintf(stderr, "Could not connect\n");
	exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    len = msg_buf -> len;
    if (len + 1> MAX_BUF_SIZE) {
	fprintf(stderr,
		"Ignoring long message\n");
	exit(EXIT_FAILURE);
    }

    for(int i=0; i < len; i++)
    {
	printf("%02x", msg_buf -> body[i]);
    }

    printf("\n");

    if (write(sfd, msg_buf -> body, len) != len) {
	fprintf(stderr, "partial/failed write\n");
	exit(EXIT_FAILURE);
    }

    nread = recv(sfd, resp_buf, MAX_BUF_SIZE-1, 0);
    if (nread == -1) {
	perror("read");
	exit(EXIT_FAILURE);
    }

    for(int i=0; i < nread; i++){
	printf("%c", resp_buf[i]);
    }
    printf("\n");
    printf("Received %zd bytes: %s\n", nread, resp_buf);

}










