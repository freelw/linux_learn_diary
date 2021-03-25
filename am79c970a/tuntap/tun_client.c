#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/if_tun.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define max(a,b) ((a)>(b) ? (a):(b))

int tun_alloc(int flags)
{
    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";
    if ((fd = open(clonedev, O_RDWR)) < 0) {
        return fd;
    }
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;
    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        close(fd);
        return err;
    }
    printf("Open tun/tap device: %s for reading/writting...\n", ifr.ifr_name);
    return fd;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("please input server address.\n");
        return -1;
    }
    int tun_fd;
    char buf[1500];
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *        IFF_NO_PI - Do not provide packet information
     */
    tun_fd = tun_alloc(IFF_TUN | IFF_NO_PI);
    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }
    int clnt_sock;
    struct sockaddr_in server;
    clnt_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (clnt_sock == -1)
    {
        printf("Could not create socket");
    }
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( 1234 );
    if (connect(clnt_sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
    printf("server conneted.\n");
    ioctl(clnt_sock, TUNSETNOCSUM, 1); 
    ioctl(tun_fd, TUNSETNOCSUM, 1); 
    fd_set fds;
    int fm = max(clnt_sock, tun_fd) + 1;
    int l = 0;
    while(1){
        FD_ZERO(&fds);
        FD_SET(clnt_sock, &fds);
        FD_SET(tun_fd, &fds);
        select(fm, &fds, NULL, NULL, NULL);
        if(FD_ISSET(clnt_sock, &fds)) {
            l = read(clnt_sock,buf,sizeof(buf));
            printf("recieved %d bytes from vpn server.\n", l);
            write(tun_fd,buf,l);
        }
        if(FD_ISSET(tun_fd, &fds)) {
            l = read(tun_fd, buf, sizeof(buf));
            printf("sending %d bytes to vpn server.\n", l);
            write(clnt_sock, buf, l);
        }
    }
    return 0;
}
