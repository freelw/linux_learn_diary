#include <stdio.h>
#include <stdlib.h>

char* getMemory()
{
    char *p = (char *)malloc(30);
    return p;
}

int main()
{
    char *p = getMemory();
    p = NULL;
    return 0;
}
