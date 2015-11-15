#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct gox_agent_conf_ctrl_s {
    uint32_t ipv4_addr;
    uint16_t port;
    uint32_t local_addr;
    uint16_t local_port;
    enum {
        tcp,
        tls
    } protocol;
} gox_agent_conf_ctrl_t;


typedef struct gox_agent_conf_br_s {
    bool enabled;
    enum {
      fail_secure_mode,
      fail_standalone_mode
    } fail_mode;
} gox_agent_conf_br_t;

typedef struct gox_agent_conf_s {
    gox_agent_conf_br_t br;

} gox_agent_conf_t;


int main() {
    
    gox_agent_conf_br_t br;
    br.fail_mode = fail_standalone_mode;
    printf("fail mode = %d\n", fail_standalone_mode);
    printf("fail mode = %d\n", br.fail_mode);
    return 0;
}
