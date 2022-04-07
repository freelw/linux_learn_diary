#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pty.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

int written(int fd, char *write_buffer, int length) {
    int send_len = 0;
    while (send_len < length) {
        int write_size = write(fd, write_buffer + send_len, length - send_len);
        if (write_size <= 0) {
            break;
        }
        send_len += write_size;
    }
    return send_len;
}

void bridge(int fd1, int fd2) {
    char buf[1024];
    fd_set fds;
    int fm = max(fd1, fd2) + 1;
    int l = 0;
    while (1) {
        FD_ZERO(&fds);
        FD_SET(fd1, &fds);
        FD_SET(fd2, &fds);
        select(fm, &fds, NULL, NULL, NULL);
        if (FD_ISSET(fd1, &fds)) {
            l = read(fd1, buf, sizeof(buf));
            if (l == 0) {
                break;
            }
            written(fd2, buf, l);
        }
        if (FD_ISSET(fd2, &fds)) {
            l = read(fd2, buf, sizeof(buf));
            if (l == 0) {
                break;
            }
            written(fd1, buf, l);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("too few args\n");
        return -1;
    }
    int ptm;
    pid_t pid = forkpty(&ptm, NULL, NULL, NULL);
    if (pid == -1) {
        printf("fork error\n");
        return -1;
    }
    if (pid == 0) {
        if (execvp(argv[1], &argv[1]) < 0) {
            printf("exec failed.\n");
            return -1;
        }
    } else {
        bridge(1, ptm);
    }
    return 0;
}
