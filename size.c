#include <stdio.h>
#include <string.h>


struct data {
    int x;
    char y[123];
    int changed[];
};


void test(int changed[]) {

    printf("changed addr %ld\n", sizeof(changed));

}

int main() {

    struct data my_data;    /* Bits set to 1 for columns that changed. */
    struct data *ptr;    /* Bits set to 1 for columns that changed. */
    *my_data.changed = 1;
    
    ptr = &my_data;
    test(ptr->changed);
    printf("data %ld\n", sizeof(my_data));
    printf("changed value %ld\n", sizeof(*ptr->changed));

    return 0;
}
