#include <stdio.h>


int main() {

    char rlt[1024] = {0};

    printf("rlt = %s\n", rlt);

    sprintf(rlt, "test %d %s", 100, "I lova Jessica");

    printf("rlt = %s\n", rlt);
    return 0;
}
