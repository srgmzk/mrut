/*
 * =====================================================================================
 *
 *       Filename:  layer2.h
 *
 *    Description:  This file define the structures required for layer 2 functionality
 *
 *        Version:  1.0
 *        Created:  07/06/2020 12:41:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef __LAYER2__
#define __LAYER2__

#include "../net.h"
#include "../utils.h"
#include "../graph.h"
#include "../comm.h"
#include "../tcpconst.h"
#include "../gluethread/glthread.h"

#include <string.h>
#include <stdlib.h>


#pragma pack (push, 1) /* now compiler do not insert any padding bytes in between the members of these data */
typedef struct arp_hdr_ {
	short hw_type;          /* 1 for ehternet cable */
    short proto_type;       /* 0x0800 for IPv4 */
    char hw_addr_len;       /* 6 for MAC */
    char proto_addr_len;    /* 4 for IPv4 */
    short op_code;          /* req or reply */
    mac_addr_t src_mac;      /* MAC of OIF interface */
    unsigned int src_ip;    /* IP of OIF */
    mac_addr_t dst_mac;      /*  */
    unsigned int dst_ip;    /* IP for which ARP is being resolved */
} arp_hdr_t;


typedef struct ehternet_hdr_{
	mac_addr_t dst_mac;
	mac_addr_t src_mac;
	unsigned short type;
	char payload[248]; /* max allowed 1500 */
	unsigned int FCS;
} ethernet_hdr_t;
#pragma pack(pop)

#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t*)0)->payload))

//#define ETH_FCS(eth_hdr_ptr, payload_size) (eth_hdr_ptr->FCS) // -- ? impl look above 

#define ETH_FCS(eth_hdr_ptr, payload_size)  \
    (*(unsigned int *)(((char *)(((ethernet_hdr_t *)eth_hdr_ptr)->payload) + payload_size)))

static inline ethernet_hdr_t *
ALLOC_ETH_HDR_WITH_PAYLOAD(char *pkt, unsigned int pkt_size) {
	ethernet_hdr_t* ether_hdr;
	ether_hdr = calloc(1, sizeof(ethernet_hdr_t));
	memcpy(ether_hdr->payload, pkt, pkt_size);
	return ether_hdr;	
}


static inline bool_t 
l2_frame_recv_qualify_on_interface(interface_t* interface, ethernet_hdr_t *ethernet_hdr) {


	if (!IS_INTF_L3_MODE(interface)) 
		return 0;


	/*return TRUE if receiving machine must accept the frame*/
	if(memcmp(IF_MAC(interface), ethernet_hdr->dst_mac.mac_val, sizeof(mac_addr_t)) == 0) {
		return 1;
	}
	
	if(IS_MAC_BROADCAST_ADDR(ethernet_hdr->dst_mac.mac_val)) {
		return 1;
	}

	return 0;
			
}

/*ARP Table APIs*/
typedef struct arp_table_{
	glthread_t arp_entries;
} arp_table_t;

typedef struct arp_entry_{
	ip_addr_t ip_addr; /*key*/
	mac_addr_t mac_addr;
	char oif_name[IF_NAME_SIZE];
	glthread_t arp_glue;
	
} arp_entry_t;
GLTHREAD_TO_STRUCT(arp_glue_to_arp_entry, arp_entry_t, arp_glue);

void
init_arp_table(arp_table_t **arp_table);

arp_entry_t *
arp_table_lookup(arp_table_t *arp_table, char *ip_addr);

void
clear_arp_table(arp_table_t *arp_table);

void
delete_arp_table_entry(arp_table_t *arp_table, char *ip_addr);

bool_t
arp_table_entry_add(arp_table_t *arp_table, arp_entry_t *arp_entry);

void
dump_arp_table(arp_table_t *arp_table);

void
arp_table_update_from_arp_reply(arp_table_t *arp_table, arp_hdr_t *arp_hdr, interface_t *iif);

void
send_arp_broadcast_request(node_t *node, interface_t *oif, char *ip_addr);
	
void
node_set_intf_l2_mode(node_t *node, char *intf_name, intf_l2_mode_t intf_l2_mode);
	
/*fn to get access to ethernet payload address*/
static inline char *
GET_ETHERNET_HDR_PAYLOAD(ethernet_hdr_t *ethernet_hdr){

      return ethernet_hdr->payload;
}

/* VLAN support */
#pragma pack(push, 1)
/* Vlan 802.1q 4 byte hdr */
typedef struct vlan_8021q_hdr_{
	unsigned short tpid; /* = 0x8100 */
	short tci_pcp : 3;	 /* initial 4 bits not used in this course */
	short tci_dei : 1;	 /* not used */
	short tci_vid : 12;	 /* Tagged vlan id */
} vlan_8021q_hdr_t;

typedef struct vlan_ethernet_hdr_{
	mac_add_t dst_mac;
	mac_add_t src_mac;
	vlan_8021q_hdr_t vlan_8021q_hdr;
	unsigned short type;
	char payload[248]; /* max allowed 1500 */
	unsigned int FCS;
} vlan_ethernet_hdr_t;
#pragma pack(pop)


/* Return NULL if Input ethernet hdr is not vlan tagged, else return pointer to 801.1q vlan hdr 
embedded in ethernet hdr*/

static inline vlan_8021q_hdr_t *
is_pkt_vlan_tagged(ethernet_hdr_t* ethernet_hdr) {
	vlan_ethernet_hdr_t* vlan_eth_hdr;
	if (ethernet_hdr->type != 0x8100)	
		return NULL;
	vlan_eth_hdr = (vlan_ethernet_hdr_t *)ethernet_hdr;
	return  vlan_eth_hdr->vlan_8021q_hdr;
}

/* return vlan id present in 802.1Q vlan hdr which is passed as an argument */
static inline unsigned int
GET_802_1Q_VLAN_ID(vlan_8021q_hdr_t *vlan_8021q_hdr) {

}

/* a macro ETH_FCS returned the FCS value present in vlan ethernet hdr */
#define VLAN_ETH_FCS(vlan_eth_hdr_ptr, payload size)

/* macro for vlan tagged ethernet hdr excluding payload size */ 
#define VLAN_ETH_HDR_SIZE_EXCL_PAYLOAD


/* return the pointer to the start of the payload present in the ethernet hdr pointer passed as an argument*/
static inline char *
GET_ETHERNET_HDR_PAYLOAD(ethernet_hdr_t *ethernet_hdr) {

}




#endif /*__LAYER2__*/

