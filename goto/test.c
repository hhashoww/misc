#include <stdio.h>
void b() {
label:
    printf("b\n");
}

void a() {

    printf("a\n");
    goto label;
}


int main () {
    a();
    return 0;
}
