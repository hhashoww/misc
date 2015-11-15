#include "gox_agent_conf_shm.h"

uint64_t seqno = 0;

void gox_agent_conf_detect_change() {
    
    gox_agent_conf_t *shm = NULL;
    shm = gox_agent_conf_get_shm();
    while(1) {
        printf("agent config detecting...\n");
        if (shm->seqno != seqno) {
            // check each module's config

            printf("seqno update from %ld to %ld\n", seqno, shm->seqno);
            // update seqno
            seqno = shm->seqno;
        }
        sleep(1);
    }
    return 0;

}
