#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void print_process_id() {
    printf("Process ID: %d\n", getpid());
    printf("cat /proc/%d/maps\n", getpid());
}

void print_table_header() {
    printf("%10s | %18s | %18s\n", "Power", "malloc", "mmap");
    printf("----------------------------------------\n");
}

void test_malloc_mmap(int power, int release) {
    size_t size = 1 << power;

    void* malloc_ptr = malloc(size);
    void* mmap_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (malloc_ptr != NULL && mmap_ptr != MAP_FAILED) {
        printf("%10d | %18p | %18p\n", power, malloc_ptr, mmap_ptr);
        if (release) {
            free(malloc_ptr);
            munmap(mmap_ptr, size);
        }
    } else {
        printf("Memory allocation failed for power 2^%d.\n", power);
        if (malloc_ptr != NULL) free(malloc_ptr);
        if (mmap_ptr != MAP_FAILED) munmap(mmap_ptr, size);
    }
}

int main(int argc, char *argv[]) {
    print_table_header();
    if (argc == 2) {
        printf("wait 3s....\n");
        sleep(3);
        int power = atoi(argv[1]);
        test_malloc_mmap(power, 0);
    } else {
        for (int i = 1; i <= 20; i++) {
            test_malloc_mmap(i, 1);
        }
    }
    print_process_id();
    while (1) {
        sleep(1);
    }

    return 0;
}
