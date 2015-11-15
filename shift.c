#include <stdio.h>


enum ofp_port_state {  
    OFPPS_LINK_DOWN   = 1 << 0,
    OFPPS_BLOCKED     = 1 << 1,
    OFPPS_LIVE        = 1 << 2
};           

int main() {
    
    int state;

    state = ~OFPPS_BLOCKED;
    printf("%x\n", state);

    state = 0;
    state = OFPPS_LINK_DOWN + OFPPS_LIVE;
    printf("%x\n", state);
        
    printf("%x\n", (OFPPS_LINK_DOWN + OFPPS_LIVE) & ~OFPPS_BLOCKED);

    return 0;
}
