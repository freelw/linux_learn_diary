#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#define GLOBALMEM_MAGIC 'g'
#define MEM_CLEAR _IO(GLOBALMEM_MAGIC, 0)
#define BUFFER_LEN 20
int main(void) {
    int fd, num;
    char rd_ch[BUFFER_LEN];
    fd_set rfds, wfds;

    fd = open("/dev/globalmem0", O_RDONLY | O_NONBLOCK);
    if (fd != -1) {
        if (ioctl(fd, MEM_CLEAR, 0) < 0) {
            printf("ioctl command failed\n");
        }
        while (1) {
            FD_ZERO(&rfds);
            FD_ZERO(&wfds);
            FD_SET(fd, &rfds);
            FD_SET(fd, &wfds);
            select(fd + 1, &rfds, &wfds, NULL, NULL);
            if (FD_ISSET(fd, &rfds)) {
                printf("Poll monitor: can be read\n");
            }
            if (FD_ISSET(fd, &wfds)) {
                printf("Poll monitor: can be written\n");
            }
        }
    } else {
        printf("Device open failure\n");
    }
    return 0;
}