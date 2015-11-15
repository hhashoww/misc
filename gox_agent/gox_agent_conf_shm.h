#include "gox_agent_conf_lock.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define GOX_AGENT_CONF_SHM_KEY      0x2015
#define GOX_AGENT_BRIDGE_MAX_NUM    8
#define GOX_AGENT_CTRL_MAX_NUM      4
#define GOX_AGENT_TABLE_MAX_NUM     240
#define GOX_AGENT_PORT_MAX_NUM      1024
#define GOX_AGENT_CONF_CMD_Q_SIZE   128

typedef struct gox_agent_conf_ctrl_s {
    bool     usage;
    char     ipv4_addr[16];
    uint16_t port;
    char     local_addr[16];
    uint16_t local_port;
    char     protocol[4];
} gox_agent_conf_ctrl_t;


typedef struct gox_agent_conf_table_match_field_s {
    uint64_t in_port:1;
    uint64_t in_phy_port:1;
    uint64_t metadata:1;
    uint64_t eth_dst:1;
    uint64_t eth_src:1;
    uint64_t eth_type:1;
    uint64_t vlan_vid:1;
    uint64_t vlan_pcp:1;
    uint64_t ip_dscp:1;
    uint64_t ip_ecn:1;
    uint64_t ip_proto:1;
    uint64_t ipv4_src:1;
    uint64_t ipv4_dst:1;
    uint64_t tcp_src:1;
    uint64_t tcp_dst:1;
    uint64_t udp_src:1;
    uint64_t udp_dst:1;
    uint64_t sctp_src:1;
    uint64_t sctp_dst:1;
    uint64_t icmpv4_type:1;
    uint64_t icmpv4_code:1;
    uint64_t arp_op:1;
    uint64_t arp_spa:1;
    uint64_t arp_tpa:1;
    uint64_t arp_sha:1;
    uint64_t arp_tha:1;
    uint64_t ipv6_src:1;
    uint64_t ipv6_dst:1;
    uint64_t oxm_of_ipv6_flabel:1;
    uint64_t icmpv6_type:1;
    uint64_t icmpv6_code:1;
    uint64_t ipv6_nd_target:1;
    uint64_t ipv6_nd_sll:1;
    uint64_t ipv6_nd_tll:1;
    uint64_t mpls_label:1;
    uint64_t mpls_tc:1;
    uint64_t mpls_bos:1;
    uint64_t pbb_isid:1;
    uint64_t tunnel_id:1;
    uint64_t ipv6_exthdr:1;
} gox_agent_conf_table_match_field_t;

typedef struct gox_agent_conf_table_s {
    uint64_t seqno;
    uint8_t table_id;
    gox_agent_conf_table_match_field_t match;
} gox_agent_conf_table_t;


typedef struct gox_port_feature_conf_s {
    uint64_t seqno;
    enum {
        Gb_10,
        Gb_40,
        other
    } rate;
    uint32_t other_rate;

    enum {
        half,
        full
    } duplex;

    bool auto_negotiate;
    bool pause;
    bool pause_asym;
} gox_port_feature_conf_t;

typedef struct gox_agent_conf_port_s {
    bool        usage;
    uint64_t    seqno;
    uint8_t     port_no;
    uint8_t     hw_addr[6]; // SPEC defined
    char        name[16]; // SPEC defined
    uint32_t    config;
#if 0
    enum ofp_port_config {
        OFPPC_PORT_DOWN = 1 << 0, /* Port is administratively down. */
        OFPPC_NO_RECV = 1 << 2, /* Drop all packets received by port. */
        OFPPC_NO_FWD = 1 << 5, /* Drop packets forwarded to port. */
        OFPPC_NO_PACKET_IN = 1 << 6 /* Do not send packet-in msgs for port. */
    };
#endif
    
    gox_port_feature_conf_t advertised;

} gox_agent_conf_port_t;

typedef struct gox_agent_conf_br_s {
    uint64_t seqno;

    uint8_t bridge_id;

    char dp_desc[256];   // SPEC defined

    bool enabled;

    enum {
      standalone,
      secure
    } fail_mode;
    
    uint8_t n_ctrl;
    gox_agent_conf_ctrl_t ctrls[GOX_AGENT_CTRL_MAX_NUM];

    uint8_t n_table;
    gox_agent_conf_table_t tables[GOX_AGENT_TABLE_MAX_NUM];
    
    uint8_t n_port;
    gox_agent_conf_port_t ports[GOX_AGENT_PORT_MAX_NUM];
} gox_agent_conf_br_t;

/*
typedef struct gox_agent_conf_cmd_entry_s {
    enum gox_cli_cmd_type   cmd; // dump or set, dump or set, if cmd == "dump", need write result to dump_file
    int                     status; // the cli command handle status , success will be set to 1
    int                     used; // identify the queue index was used?
    char                    dump_file[256]; // the dump cmd output result's file
    int                     argc; // the number of command arguments, only count the args after dumr/set
    char                    argv[GOX_CLI_CMD_ARG_NUM][GOX_CLI_CMD_ARG_LEN]; // the command arguments, onln include the args after dump/set
} gox_agent_conf_cmd_entry_t;
*/
typedef struct gox_agent_conf_s {
    // config structure
    uint8_t     lock;
    uint16_t    my_lock_count;
    uint64_t    seqno;
    gox_agent_conf_br_t br[GOX_AGENT_BRIDGE_MAX_NUM];
    enum {
        ipv4,
        ipv6
    } mpls_nxt_hdr;

    // show command queue
   // int write_idx;
   // int read_idx;
   // gox_agent_conf_cmd_entry_t cmd_q[GOX_AGENT_CONF_CMD_Q_SIZE];

} gox_agent_conf_t;



/*! Get gox agent config address from share memory 
  @return address on success. NULL on failure

  @author Allen Huang 
 */
gox_agent_conf_t *gox_agent_conf_get_shm(void);

/*! Initinal agent config in share memory
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_shm_init();


/*! Dump agent config in share memory
  @return GOX_SUCCESS on success. GOX_FAILED on failure

  @author Allen Huang 
 */
int gox_agent_conf_shm_dump();
