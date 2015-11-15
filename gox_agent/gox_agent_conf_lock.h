#ifndef __GOX_AGENT_CONF_LOCK_H
#define __GOX_AGENT_CONF_LOCK_H
#include <stdint.h>
void gox_agent_conf_lock(uint8_t *ptr, uint16_t *my_lock_count);
void gox_agent_conf_unlock(uint8_t *ptr);
#endif
