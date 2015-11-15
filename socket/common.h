#include <stdint.h>
typedef struct gox_hal_flow_desc_s {
    int type;
    int x;
    int y;
}gox_hal_flow_desc_t;

typedef struct gox_pkt_in_entry_s {
    int type;
    uint8_t data[1500];
}gox_pkt_in_entry_t;


typedef 
struct gox_sysycp_agent_data {
    enum {
        flow,
        group,
        meter,
        pkt_in,
        pkt_out,
        unknown
    } data_type;
    union {
        gox_hal_flow_desc_t flow;
        gox_pkt_in_entry_t  pkt_in;
    };
} gox_syscp_agent_data_t;

