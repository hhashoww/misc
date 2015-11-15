#include <stdio.h>

typedef int (*pfnScaleCostFun)(int *pfX);

int hyc(int *chiachia) {

    printf("hyc data = %d\n", *chiachia);

    return 0;
}

int hi(int *chiachia) {

    printf("hi data = %d\n", *chiachia);

    return 0;
}

struct item {
    int data;
    int (*func)(int *);
};

int main (int argc, char **argv) {
    
    int i;
    printf("argc = %d\n", argc);
    for (i = 0 ; i < argc ; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");



    /*
    if (argc < 2)
        return -1;

    int data;
    data = atoi(argv[1]);

    struct item my_item;
    if (data > 100)
        my_item.func = hyc;
    else
        my_item.func = hi;

    my_item.data = data;

    my_item.func(&my_item.data);
*/

/*
    int (*func)(int *);
    func = hyc;
    func(&data);

    pfnScaleCostFun xxx;
    xxx = hyc;
    xxx(&data);     //hyc(123);

    */
    return 0;
}

