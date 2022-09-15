
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("arg err\n");
        return -1;
    }
    struct addrinfo hints, *res = NULL, *resNew = NULL, *last = NULL,
        *iterator;

    int error;
  

    // try once, with our static buffer
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_INET;

    error = getaddrinfo("hostname", NULL, &hints, &res);
    return 0;
}