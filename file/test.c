#include <stdio.h>
void test(char *reason) {
    sprintf(reason, "test %d\n", 100);

}

int main() {

    FILE *fp;
    char reason[256];
    fp = fopen("testfile", "w");

    if (fp == NULL)
        return -1;

    printf("start...\n");
    test(reason);
    fprintf(fp, reason);

    fclose(fp);
    return 0;
}
