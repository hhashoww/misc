#include <stdio.h>


int main() {
    char *str1 = "AAA";
    char *str2 = "BBB";
    char name[16];
    sprintf(name, "%s%s", str1, str2);
    printf("%s\n", name);
    return 0;
}
