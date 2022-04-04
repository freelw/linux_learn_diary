#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>

int get_ip_addr_from_socket(char *ip_addr) {
    int ret = 0;
    int sock;
    char *server_ip = "8.8.8.8";
    unsigned short server_port = 53;
    struct timeval timeout;
    struct sockaddr_in client;
    struct sockaddr_in server;
    socklen_t cLen = sizeof(client);
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(server_port);
    /* set timeout */
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    if (ret == -1) {
        printf("get_ip_addr_from_socket SO_SNDTIMEO error:%d\n", errno);
        close(sock);
        return -1;
    }

    ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (ret == -1) {
        printf("get_ip_addr_from_socket connect error:%d\n", errno);
        close(sock);
        return ret;
    }
    getsockname(sock, (struct sockaddr *)&client, &cLen);
    strcpy(ip_addr, inet_ntoa(client.sin_addr));
    close(sock);
    return ret;
}

int main() {
    char ip_addr[64];
    get_ip_addr_from_socket(ip_addr);
    /*int sock;
    char *server_ip = "8.8.8.8";
    unsigned short server_port = 53;
    struct sockaddr_in client;
    struct sockaddr_in server;
    int ret;
    char ip_addr[64];
    memset(&server, 0, sizeof(server));
    socklen_t cLen = sizeof(client);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(server_port);
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    printf("ret = %d\n", ret);
    ret = getsockname(sock, (struct sockaddr *)&client, &cLen);
    printf("ret = %d\n", ret);
    printf("%d\n", inet_ntoa(client.sin_addr));
    strcpy(ip_addr, inet_ntoa(client.sin_addr));
    printf("%s\n", ip_addr);*/
    return 0;
}