/*
 * =====================================================================================
 *
 *       Filename:  net.h
 *
 *    Description:  This file contains all definitions for structures required for net prog
 *
 *        Version:  1.0
 *        Created:  05/19/2020 12:34:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  srg mos 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __NET__
#define __NET__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


typedef int bool_t;
typedef struct graph_ graph_t;
//Forward declaration
typedef struct node_ node_t;
typedef struct link_ link_t;
typedef struct interface_ interface_t;

typedef struct ip_addr_ {
	char ip_val[16];
} ip_addr_t;

typedef struct mac_addr_ {
	char mac_val[18];
} mac_addr_t;

/*Forward Declaration*/
typedef struct arp_table_ arp_table_t;

typedef struct node_nw_props_ {

	/* Used to find various device types capabilities of  
	 * the node and other features */
	unsigned int flags;
	
	/* L2 capabilities */
	arp_table_t *arp_table;

	/*L3 properties*/
	bool_t is_lb_addr_config;
	ip_addr_t lb_addr; /* loopback address of node */
} node_nw_props_t;


extern void
init_arp_table(arp_table_t **arp_table);

static inline void
init_node_nw_props(node_nw_props_t *node_nw_props) {
	node_nw_props->flags = 0;
	node_nw_props->is_lb_addr_config = false;
	memset(node_nw_props->lb_addr.ip_val, 0, 16);
	init_arp_table(&(node_nw_props->arp_table));
}

typedef struct intf_nw_props_ {
	/* L2 properties */
	mac_addr_t mac_addr; /*MAC are hard burnt in interface NIC*/
	/* L3 properties */
	bool_t is_ipaddr_config; /*set to TRUE if ip addr is configured, intf operates in L3 mode if ip address is configured on it */
	ip_addr_t ip_addr;
	char mask;
} intf_nw_props_t;

static inline void 
init_intf_nw_props(intf_nw_props_t *intf_nw_props) {

	memset(intf_nw_props->mac_addr.mac_val, 0, 18);
	intf_nw_props->is_ipaddr_config = false;
	memset(intf_nw_props->ip_addr.ip_val,0,16);
	intf_nw_props->mask = 0;
	
	
}

unsigned int convert_ip_from_str_to_int(char *ip_addr);
void convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer);
interface_t* node_get_matching_subnet_interface(node_t *node, char *ip_addr);
void dump_nw_graph(graph_t *graph);
void *pkt_buffer_shift_right(char* pkt, int pkt_size, int empty_size);


/*GET shorthand Macros*/

#define IF_NAME(intf_ptr) (intf_ptr->if_name)
#define IF_MAC(intf_ptr) (intf_ptr->intf_nw_props.mac_addr.mac_val)
#define IF_IP(intf_ptr)	 (intf_ptr->intf_nw_props.ip_addr.ip_val)
#define NODE_LO_ADDR(node_ptr) (node_ptr->node_nw_props.lb_addr.ip_val)

/* 
	evaluate to TRUE, if ip address is configured on an interface.
	Interface is said to be operating in L3 mode if it has an IP address configured.
 */

#define IS_INTF_L3_MODE(intf_ptr) (intf_ptr->intf_nw_props.is_ipaddr_config) 

/*APIs to set Network Node properties*/
void interface_assign_mac_address(interface_t *interface);
bool_t node_set_loopback_address(node_t *node, char *ip_addr);
bool_t node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask);
bool_t node_unset_intf_ip_address(node_t *node, char *local_if);
	

#endif /*__NET__*/

