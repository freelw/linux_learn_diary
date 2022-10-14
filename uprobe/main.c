#include <stdio.h>
#include <string.h>
int func1() {

    printf("1\n");
}

int main() {


    
    func1();
    malloc(1);
    printf("malloc\n");
    
    return 0;
}