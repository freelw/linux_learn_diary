
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
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("arg err\n");
        return -1;
    }
    struct addrinfo hints, *rp = NULL, *result = NULL, *resNew = NULL, *last = NULL,
        *iterator;

    int error;
    char str[INET_ADDRSTRLEN] = {0};
  

    // try once, with our static buffer
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_INET;

    for (int i = 0; i < 1; i ++ )
        error = getaddrinfo(argv[1], NULL, &hints, &result);
        printf("error %d\n", error);
        if (error == 0) {
        for (rp = result; rp != NULL; rp = rp->ai_next) {

            printf("%d %d %d\n", rp->ai_family, rp->ai_socktype,rp->ai_protocol);
            inet_ntop(AF_INET, &((struct sockaddr_in *)(rp->ai_addr))->sin_addr.s_addr, str, INET_ADDRSTRLEN);
            printf("ip %s\n", str);
        
        }
    }
    return 0;
}