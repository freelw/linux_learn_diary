#include <stdio.h>

extern char **environ;

int main(void) {
    int nIndex = 0;
    for (nIndex = 0; environ[nIndex] != NULL; nIndex++) {
        printf("%s\n", environ[nIndex]);
    }
    return 0;
}