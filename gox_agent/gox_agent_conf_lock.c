#include <time.h>
#include <stdint.h>
#include <stdio.h>

void gox_agent_conf_lock(uint8_t *ptr, uint16_t *my_lock_count)
{

    time_t t;
    t = time(NULL);
    uint16_t pre_lock_count = *my_lock_count;
	while (1) {
		if (__sync_bool_compare_and_swap(ptr, 0, 1))
			break;
        if (pre_lock_count != *my_lock_count) {
            pre_lock_count = *my_lock_count;
            t = time(NULL);
            continue;
        }
        if (time(NULL) - t > 5) {
            *ptr = 1;
            break;
        }
		usleep(1);
	}
	(*my_lock_count) ++;
}

void gox_agent_conf_unlock(uint8_t *ptr)
{
	*ptr=0;
}

