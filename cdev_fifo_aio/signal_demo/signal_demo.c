#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <signal.h>

static void signalio_handler(int signum) {
    printf("receive a signal from globalmem, signalnum:%d\n", signum);
}

int main(void) {
    int fd, oflags;
    fd = open("/dev/globalmem0", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd != -1) {
        signal(SIGIO, signalio_handler);
        fcntl(fd, F_SETOWN, getpid());
        oflags = fcntl(fd, F_GETFL);
        fcntl(fd, F_SETFL, oflags | FASYNC);
        while (1) {
            sleep(100);
        }
    } else {
        printf("Device open failure\n");
    }
    return 0;
}