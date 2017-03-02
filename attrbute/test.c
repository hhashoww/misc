#include <stdio.h>
#include <stdint.h>

void first_proc(void) __attribute__((constructor(101)));
void second_proc(void) __attribute__((constructor(102)));

void first_close(void) __attribute__((destructor(101)));
void second_close(void) __attribute__((destructor(102)));


int test() {
	printf("test\n");
}

void first_proc(void) {

	printf("first run !\n");
}


void second_proc(void) {
	printf("second run !\n");
}




void first_close(void) {
	printf("first close !\n");
}


void second_close(void) {
	printf("second close !\n");
}




