#include "gox_agent_conf_shm.h"

/* Bridge */
int gox_agent_conf_br_enable_set(int bridge_id, int state, char *reason) {

    gox_agent_conf_t    *shm = NULL;
    gox_agent_conf_br_t *br;

    if (bridge_id < 0 | bridge_id > 8) {
        sprintf(reason, "bridge id %d failed", bridge_id);
        return -1;
    }

    if (state != 0 && state != 1) {
        sprintf(reason, "state %d failed", state);
        return -1;
    }

    shm = gox_agent_conf_get_shm();

    if (shm == NULL) {
        sprintf(reason, "can't get the configure manager");
        return -1;
    }


    br = &shm->br[bridge_id];
    if (br->enabled == state) {
        return 0;
    }

    gox_agent_conf_lock(&shm->lock, &shm->my_lock_count);

    br->enabled = state;
    br->seqno++;
    shm->seqno++;

    gox_agent_conf_unlock(&shm->lock);

    return 0;
}


/*! Set logical switch's description
  @param[in] bridge_id Specify the bridge id.
  @param[in] desc Specify the description of this logical switch..
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_desc_set(int bridge_id, char *desc, char *reason);
// set br0 desc xxx [strlen :  256] 

/*! Set logical switch's fail mode
  @param[in] bridge_id Specify the bridge id.
  @param[in] fail_mode Specify the fail mode, secure(0) or standalone(1).
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_fail_mode_set(int bridge_id, int fail_mode, char *reason);  // 0:secure, 1:standalone
// set br0 fail_mode fail_secure_mode


/* Controller */
/*! Add controller into the specified bridge
  @param[in] bridge_id Specify the bridge id.
  @param[in] ipv4_addr Specify the controller ipv4 address.
  @param[in] port Specify the controller's listen port. Default is 6653
  @param[in] local_addr Specify the local side ipv4 address. Default is "0.0.0.0"
  @param[in] local_port Specify the local side port. Default is 0
  @param[in] protocol Specify the transport protocol, Default is "tcp"
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_ctrl_add(int bridge_id, char *ipv4_addr, uint16_t port, char *local_addr, uint16_t local_port, char *protocol, char *reason) {

    int idx;
    gox_agent_conf_t        *shm  = NULL;
    gox_agent_conf_br_t     *br   = NULL;
    gox_agent_conf_ctrl_t   *ctrl = NULL;;

    if (bridge_id < 0 | bridge_id > 8) {
        sprintf(reason, "bridge id %d failed", bridge_id);
        return -1;
    }

    shm = gox_agent_conf_get_shm();

    if (shm == NULL) {
        sprintf(reason, "can't get the configure manager");
        return -1;
    }

    br = &shm->br[bridge_id];
    if (br->n_ctrl == GOX_AGENT_CTRL_MAX_NUM) {
        sprintf(reason, "the maximum number of controller is %d", GOX_AGENT_CTRL_MAX_NUM);
        return -1;
    }

    gox_agent_conf_lock(&shm->lock, &shm->my_lock_count);
    for (idx = 0 ; idx < GOX_AGENT_CTRL_MAX_NUM ; idx++) {
        if (br->ctrls[idx].usage == false) {
            ctrl = &br->ctrls[idx];
            break;
        }
    }

    if (ctrl == NULL) {
        sprintf(reason, "can't get empty controller entry");
        return -1;
    }


    br->n_ctrl++;
    ctrl->usage      = true;
    ctrl->port       = port;
    ctrl->local_port = local_port;

    strcpy(ctrl->ipv4_addr, ipv4_addr);
    strcpy(ctrl->local_addr, local_addr);
    strcpy(ctrl->protocol, protocol);

    br->seqno++;
    shm->seqno++;

    gox_agent_conf_unlock(&shm->lock);

    return 0;
}

/*! Remove the specify controller from the specified bridge
  @param[in] bridge_id Specify the bridge id.
  @param[in] ipv4_addr Specify the controller ipv4 address.
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_ctrl_del(int bridge_id, uint32_t ipv4_addr, char *reason); 
// no set controller <X:X::X:X> ( port <1-65535> | local-address <X:X::X:X> | local-port <1-65535> | protocols [tls | tcp] ) controller <A.B.C.D> ( port <1-65535> | local-address <A.B.C.D> | local-port <1-65535> | protocols [tls | tcp] )

/* Table */
/*! Add the specify table to the specified bridge
  @param[in] bridge_id Specify the bridge id.
  @param[in] table_id Specify the openflow table id.
  @param[in] matchs Specify the match fields of this openflow table.
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_table_add(int bridge_id, uint8_t table_id, uint64_t matchs, char *reason);  //gox_agent_conf_table_match_field_t
// set br0 table 1 feature-matchkey in_port eth_src

/*! Remove the specify openflow table from the specified bridge
  @param[in] bridge_id Specify the bridge id.
  @param[in] table_id Specify the openflow table id.
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_table_del(int bridge_id, uint8_t table_id, char *reason);
// no set br0 table 1 feature-matchkey in_port eth_src



/* Port */
/*! Add the specify interface the specified bridge as an openflow port
  @param[in] bridge_id Specify the bridge id.
  @param[in] intf Specify the interface name.
  @param[in] port_no Specify the openflow port number.
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_port_add(int bridge_id, char *intf, uint8_t port_no, char *reason);
// set br0 interface eth0 port_no 1

/*! Remove the specify interface from specified bridge 
  @param[in] bridge_id Specify the bridge id.
  @param[in] port_no Specify the openflow port number.
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_port_del(int bridge_id, uint8_t port_no, char *reason);
// no set br0 interface eth0 port_no 1

/*! Set logical switch's port status
  @param[in] intf Specify the interface name.
  @param[in] state Specify the status of the openflow port. disable(0) or enable(1).
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_port_status_set(char *intf, int state, char *reason);
// set interface eth0 shutdown



int main() {
    
    char reason[256];
    gox_agent_conf_t *shm = NULL;
    shm = gox_agent_conf_get_shm();

    gox_agent_conf_shm_init();

    if(gox_agent_conf_br_enable_set(0, 1, reason))
        printf("%s\n", reason);
    if(gox_agent_conf_br_ctrl_add(0, "192.168.11.78", 6633, "0.0.0.0", 0, "tcp", reason))
        printf("%s\n", reason);
    if(gox_agent_conf_shm_dump())
        printf("%s\n", reason);
    return 0;

}
