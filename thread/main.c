#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

uint32_t t1_data;
uint32_t t2_data;

void *t1_func(void *arg) {

    while(t1_data < 10) {

        printf("I'm t1_data %d\n", t1_data++);
        sleep(1);
    }
}

void *t2_func(void *arg) {

    while(t2_data < 10) {

        printf("I'm t2_data %d\n", t2_data++);
        sleep(2);
    }
}


int main() {

    pthread_t t1, t2;

    t1_data = 0;
    t2_data = 0;

    pthread_create(&t1, NULL, t1_func, NULL);
    pthread_create(&t2, NULL, t2_func, NULL);

    // while(1) {
    printf("I'm main start!\n");
    //sleep(5);
    // }

    printf("join t1\n");
    pthread_join(t1, NULL);
    printf("join t2\n");
    pthread_join(t2, NULL);

    printf("I'm main end!\n");
    return 0;
}
