#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "genie_timer.h"

void tick_tmnt_f(void *arg) {
    printf("OVER\n");
}
void timeout_f(void *idx) {
    printf("KING %d\n", (*(int *)idx)++);
}

void intf(void *idx) {
    *(int *)idx = 0;
    printf("YOYO\n");
}

int main() {
    int idx;
    /*  timer tick
    timer_tick_t     timer_tick;
    timer_tick_init(&timer_tick, 1000, &intf, &idx, &timeout_f, &idx);

    while(idx < 10) {
        sleep(3);
    }
    timer_tick_terminate(&timer_tick, &tick_tmnt_f, NULL, GT_TRUE);
    */

    /* timer list
    timer_list_t     timer_list;
    timer_list_entry_t     timer_list_entry_0;
    timer_list_entry_t     timer_list_entry_1;
    timer_list_entry_t     timer_list_entry_2;
    timer_list_entry_t     timer_list_entry_3;
    timer_list_entry_t     timer_list_entry_4;
    timer_list_entry_t     timer_list_entry_5;
    timer_list_entry_t     timer_list_entry_6;
    timer_list_entry_t     timer_list_entry_7;
    timer_list_entry_t     timer_list_entry_8;
    timer_list_entry_t     timer_list_entry_9;
    timer_list_entry_t     timer_list_entry_10;
    timer_list_entry_t     timer_list_entry_11;
    timer_list_init(&timer_list, 1000, &intf, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_0, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_1, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_2, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_3, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_4, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_5, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_6, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_7, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_8, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_9, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_10, &timeout_f, &idx);
    timer_list_add_entry(&timer_list, &timer_list_entry_11, &timeout_f, &idx);
    while(idx < 10) {
        sleep(3);
    }
    timer_list_terminate(&timer_list, &tick_tmnt_f, NULL, GT_TRUE);
    */

    uint8_t	clock_slot_exp[NUM_CLOCK];
    uint8_t i = 0;
    clock_slot_exp[0] = 4;
    clock_slot_exp[1] = 4;
    clock_slot_exp[2] = 1;
    clock_slot_exp[3] = 1;
    timer_slot_t timer_slot; 
    timer_slot_entry_t timer_slot_entry[12]; 

    timer_slot_init(&timer_slot, 1, &intf, &idx);
    timer_slot_init_advance(&timer_slot, 150, &intf, &idx, clock_slot_exp);
    for (i = 0 ; i < 12 ; i++) {
        timer_slot_add_entry(&timer_slot, &timer_slot_entry[i], i, &timeout_f, &idx);
    }

    while(idx < 10) {
        sleep(3);
    }

    timer_slot_terminate(&timer_slot, &tick_tmnt_f, NULL);
	return 0;
}
