#ifndef __GOX_AGENT_CONF_CALLBACK_H
#define __GOX_AGENT_CONF_CALLBACK_H
#include "gox_agent_conf_shm.h"
/* Bridge */
/*! Set logical switch's status
  @param[in] bridge_id Specify the bridge id.
  @param[in] state Specify the status of this logical switch. disable(0) or enable(1).
  @paran[out] reason if failed, specify the fail reason
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_br_enable_set(int bridge_id, int state, char *reason); 
// set br0 enabled 


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
  @param[in] fail_mode Specify the fail mode, standalone(0) or secure(1). Default is standalone(0).
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
int gox_agent_conf_br_ctrl_add(int bridge_id, char *ipv4_addr, uint16_t port, char *local_addr, uint16_t local_port, char *protocol, char *reason);
// set controller <X:X::X:X> ( port <1-65535> | local-address <X:X::X:X> | local-port <1-65535> | protocols [tls | tcp] ) controller <A.B.C.D> ( port <1-65535> | local-address <A.B.C.D> | local-port <1-65535> | protocols [tls | tcp] )


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
#endif
