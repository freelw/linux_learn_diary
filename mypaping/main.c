#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>

void setnonblocking(int sock)
{
     int opts;
     opts=fcntl(sock,F_GETFL);
     if(opts<0)
     {
          perror("fcntl(sock,GETFL)");
     }
     opts = opts|O_NONBLOCK;
     if(fcntl(sock,F_SETFL,opts)<0)
     {
          perror("fcntl(sock,SETFL,opts)");
     }  
}


int paping() {
    int ret = 0;
    int sock;
    char *server_ip = "172.17.16.14";
    unsigned short server_port = 3306;
    struct sockaddr_in client;
    struct sockaddr_in server;
    fd_set	read, write;
    struct timeval tv;
    int result;
    socklen_t cLen = sizeof(client);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    setnonblocking(sock);
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(server_port);   
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    tv.tv_sec 	= 0;
	tv.tv_usec	= 1000 * 1000;
	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_SET(sock, &read);
	FD_SET(sock, &write);
    result = select(sock + 1, &read, &write, NULL, &tv);
    printf("result = %d\n", result);
    if (!FD_ISSET(sock, &read) && !FD_ISSET(sock, &write)) {
		printf("err\n");
        ret = -1;
	} else {
        printf("succ\n");
    }
    close(sock);
    return ret;
}

int main() {
    /*while (1) {
        paping();
        usleep(1000000);
    }*/
    return paping();
}