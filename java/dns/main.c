#include<stdio.h>
#include<netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    struct hostent *ghbn=gethostbyname("wangli.com");//change the domain name
    if (ghbn) {
        printf("Host Name->%s\n", ghbn->h_name);
        printf("IP ADDRESS->%s\n",inet_ntoa(*(struct in_addr *)ghbn->h_name) );
    }
}