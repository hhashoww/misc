#include <stdio.h>


int main() {
    int data = 101;
    switch (data) {
        case 1:
            printf("1\n");
            break;
        case 101:
            printf("101\n");
            break;
        case 10:
            printf("10\n");
            break;
        default:
            printf("default\n");
            break;
    }
    return 0;

}
