#include <stdio.h>
#include <unistd.h>

int count = 0;
int b() {
    count++;
}
int a() {
    b();
}

int main() {
    pid_t pid = getpid();
    char *cmd_tempalte = "perf cmd:\n"
                         "\tsudo perf record -F 99 -p %d -g -- sleep 30\n"
                         "\tsudo perf report\n";

    printf(cmd_tempalte, pid);
    while (1) {
        for (int i = 0; i < 100000000; ++i) a();
    }
    return 0;
}