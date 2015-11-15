#include "gox_agent_conf_shm.h"


int gox_agent_conf_shm_init() {

    int br_idx;
    gox_agent_conf_t    *shm = NULL;

    shm = gox_agent_conf_get_shm();
    if (shm == NULL) {
        //sprintf(reason, "can't get the configure manager");
        return -1;
    }

	memset(shm, 0, sizeof(gox_agent_conf_t));
	
    for (br_idx = 0 ; br_idx < GOX_AGENT_BRIDGE_MAX_NUM ; br_idx++) {
        shm->br[br_idx].bridge_id = br_idx;
    }
	return 0;
}

static gox_agent_conf_t *local_ptr = NULL;

gox_agent_conf_t *gox_agent_conf_get_shm(void) 
{
	int shm_id;
	gox_agent_conf_t *shm;
	uint8_t flag=0;

	//if the process had get shared memory before
	if(local_ptr != NULL) {
		return local_ptr;
	}

	//get shared memory
	if ((shm_id = shmget(GOX_AGENT_CONF_SHM_KEY, sizeof(gox_agent_conf_t), 0666)) < 0) {
		//INSPECT(printf, ("shared memory need create\n"));
		flag=1;		
		//create shared memory
		if ((shm_id = shmget(GOX_AGENT_CONF_SHM_KEY, sizeof(gox_agent_conf_t), IPC_CREAT|IPC_EXCL|0666)) < 0) {
			//if shared memory key had use before, wait 1 second then get again
			sleep(1);
			if ((shm_id = shmget(GOX_AGENT_CONF_SHM_KEY, sizeof(gox_agent_conf_t), 0666)) < 0) {
				//if still can't get shared memory, return NULL
				//INSPECT(printf, ("shmget error\n"));
				return NULL;
			}
			// if we can't get shared memory at first, and we also can't create shared memory,
			// it's mean maybe have someone create at the same time
			// so we don't need to initial configuration again
			flag = 0;	
		}
	}

	//at tach shared memory
	if( (shm = (gox_agent_conf_t *)shmat(shm_id, NULL, 0)) == (void*)-1 ) {
		//INSPECT(printf, ("shmat error\n"));
		//if attach shared memory error twice, return NULL
		return NULL;
	}

	//if we create the shared memory, we need initial it.
	if (flag == 1) {
		if (gox_agent_conf_shm_init() != 0)
			return NULL;
	}

	local_ptr = shm;
	return shm;	
}

inline void gox_agent_conf_shm_ctrl_dump(gox_agent_conf_br_t *br) {

    uint8_t  ctrl_idx;
    uint16_t cnt;
    gox_agent_conf_ctrl_t *ctrl;
    cnt = 0;
    printf("number of controllers: %d\n", br->n_ctrl);
    for (ctrl_idx = 0 ; ctrl_idx < GOX_AGENT_CTRL_MAX_NUM ; ctrl_idx++) {
       if (br->ctrls[ctrl_idx].usage) {
           ctrl = &br->ctrls[ctrl_idx];
           printf("ipv4_addr: %s\n", ctrl->ipv4_addr); 
           printf("port: %d\n", ctrl->port); 
           printf("local_addr: %s\n", ctrl->local_addr); 
           printf("local_port: %d\n", ctrl->local_port); 
           printf("protocol: %s\n", ctrl->protocol); 
           cnt++;
       }
       if (cnt == br->n_ctrl)
           break;
    }

}

inline void gox_agent_conf_shm_port_dump(gox_agent_conf_br_t *br) {

    uint16_t port_idx;
    uint16_t cnt;
    gox_agent_conf_port_t *port;
    cnt = 0;
    printf("number of ports: %d\n", br->n_port);
    for (port_idx = 0 ; port_idx < GOX_AGENT_PORT_MAX_NUM ; port_idx++) {
        if (br->ports[port_idx].usage) {
            port = &br->ports[port_idx];
            printf("name: %s\n", port->name);
            printf("port_no: %ld\n", port->port_no);
            cnt++;
        }

        if (cnt == br->n_port)
            break;
    }

}

inline void gox_agent_conf_shm_br_dump(gox_agent_conf_br_t *br) {


    printf("=== bridge id : %d ===\n", br->bridge_id);
    printf("seqno : %ld\n", br->seqno);
    if (br->enabled)
        printf("state: enable\n");
    else
        printf("state: disable\n");
    printf("dp_desc = %s\n", br->dp_desc);

    if (br->fail_mode)
        printf("fail_mode: secure\n");
    else
        printf("fail_mode: standalone\n");

    gox_agent_conf_shm_ctrl_dump(br);
    gox_agent_conf_shm_port_dump(br);
    

}

int gox_agent_conf_shm_dump() {

    gox_agent_conf_t    *shm = NULL;
    gox_agent_conf_br_t *br;
    int br_idx;
    shm = gox_agent_conf_get_shm();
    if (shm == NULL) {
        //sprintf(reason, "can't get the configure manager");
        return -1;
    }

    printf("seqno = %ld\n", shm->seqno);
    for (br_idx = 0 ; br_idx < GOX_AGENT_BRIDGE_MAX_NUM ; br_idx++) {
        br = &shm->br[br_idx];
        gox_agent_conf_shm_br_dump(br);
    }
    return 0;
}
