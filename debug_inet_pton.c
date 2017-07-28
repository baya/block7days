#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int domain, s;
    unsigned char buf[sizeof(struct in6_addr)];
    char *src = "::ffff:127.0.0.1";
    char str[INET6_ADDRSTRLEN];

    printf("++++++++++++%lu\n", sizeof(struct in6_addr));

    domain = AF_INET6;
    s = inet_pton(domain, src, buf);
    printf("????????????:%s", buf);
    if (s <= 0) {
	if (s == 0)
	    fprintf(stderr, "Not in presentation format");
	else
	    perror("inet_pton");
	exit(EXIT_FAILURE);
    }

    if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
	perror("inet_ntop");
	exit(EXIT_FAILURE);
    }

    printf("%s\n", str);
    //printf("%lu\n", sizeof(struct in6_addr));
    exit(EXIT_SUCCESS);
}
