#include <stdio.h>
#include <algorithm>

int a[10]={2,3,1};

void print_arr(int arr[], int len) {
    for (int i = 0; i < len; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void test_sort() {
    print_arr(a, 10);
    std::sort(a, a+10);
    print_arr(a, 10);
}