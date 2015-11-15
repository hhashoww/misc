#include <string.h>
#include <stdio.h>


int main() {
    char data[] = "br6";

    printf("%s\n", data);


    char *str, *check;

    printf("%s\n", strtok_r(data, "br", &check));
    printf("%s\n", check);
    return 0;


}
