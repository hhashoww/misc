#include <stdio.h>
#include <stdint.h>

typedef union gox_monitor_event_u {
    uint64_t data[2];
    struct {
        uint8_t type;
        struct {
            uint64_t flow_cookie;
            uint8_t  reason;
        } flow_timeout;
        struct {
            uint8_t  bridge_id;
            uint32_t port_no;
            uint8_t  state;
        } port_link_change;
    } event;
} gox_monitor_event_t;



gox_monitor_event_t m_queue[128];


typedef 
struct gox_sysycp_agent_data {
    uint8_t type;
    union {
        int flow;
        int  pkt;
    };
} gox_syscp_agent_data_t;

gox_syscp_agent_data_t sa_queue[128];

int main() {

    int i = 0;
    int type;
    srand(time(NULL));

    /*
    for (i = 0 ; i < 100 ; i++) {
        type = rand()%2;
        m_queue[i].event.type = type;
    }
    for (i = 0 ; i < 100 ; i++) {
        if (m_queue[i].event.type == 1)
            printf("port\n");
        else
            printf("flow\n");
    }
    */

    for (i = 0 ; i < 100 ; i++) {
        type = rand()%2;
        sa_queue[i].type = type;
        if (type == 1) {
            sa_queue[i].pkt = i;
        } else {
            sa_queue[i].flow = i;
        }
    }

    for (i = 0 ; i < 100 ; i++) {
        if (sa_queue[i].type == 1) {
            printf("%d\n", sa_queue[i].pkt);
        }else{
            printf("%d\n", sa_queue[i].flow);
        }
    }
    return 0;
}
