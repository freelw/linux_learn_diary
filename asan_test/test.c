#include <stdio.h>
#include <stdlib.h>

char* getMemory()
{
    char *p = (char *)malloc(30);
    return p;
}

void leak() {

    char *p = getMemory();
    p = NULL;
}


void freeagain(char *p) {
    free(p);
}

void doublefree() {
    char *p = malloc(10);
    free(p);
    for (int i = 0; i < 10; ++ i) {
        printf("123\n");
    }
    for (int i = 0; i < 10; ++ i) {
        printf("1235\n");
    }
    freeagain(p);
}

int main()
{
    doublefree();
    return 0;
}
