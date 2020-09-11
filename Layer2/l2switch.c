/*
 * =====================================================================================
 *
 *       Filename:  l2switch.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/26/2020 02:48:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include "../graph.h"
#include "layer2.h"
#include "../gluethread/glthread.h"

/*L2 switch owns mac table*/

typedef struct mac_table_entry_{
	mac_addr_t mac;
	char oif_name[IF_NAME_SIZE];
	glthread_t mac_entry_glue;
} mac_table_entry_t;

GLTHREAD_TO_STRUCT(mac_entry_glue_to_mac_entry, mac_table_entry_t, mac_entry_glue);

typedef struct mac_table_{
	glthread_t mac_entries;
} mac_table_t;

void init_mac_table(mac_table_t **mac_table) {
	*mac_table = calloc(1, sizeof(mac_table_t));
	init_glthread(&((*mac_table)->mac_entries));
}

mac_table_entry_t *
mac_table_lookup(mac_table_t *mac_table, char *mac) {

	glthread_t *curr;
	mac_table_entry_t *mac_table_entry;
	ITERATE_GLTHREAD_BEGIN(&mac_table->mac_entries, curr) {
		mac_table_entry = mac_entry_glue_to_mac_entry(curr);
	if(strncmp(mac_table_entry->mac.mac_val, mac, sizeof(mac_addr_t)) == 0 ) {
			return mac_table_entry;
		}
	} ITERATE_GLTHREAD_END(&mac_table->mac_entries, curr);
	return NULL; 
}

void
clear_mac_table(mac_table_t *mac_table){
	glthread_t *curr;
	mac_table_entry_t *mac_table_entry;

	ITERATE_GLTHREAD_BEGIN(&mac_table->mac_entries, curr) {
		mac_table_entry = mac_entry_glue_to_mac_entry(curr);
		remove_glthread(curr);
		free(mac_table_entry);
	} ITERATE_GLTHREAD_END(&mac_table->mac_entries, curr);
}

void
delete_mac_table_entry(mac_table_t *mac_table, char *mac) {
	mac_table_entry_t *mac_table_entry;
	mac_table_entry = mac_table_lookup(mac_table, mac);
	if(!mac_table_entry)
		return;
	remove_glthread(&mac_table_entry->mac_entry_glue);
	free(mac_table_entry);
}

#define IS_MAC_TABLE_ENTRY_EQUAL(mac_entry_1, mac_entry_2) \
	( strncmp( (mac_entry_1->mac.mac_val), (mac_entry_2->mac.mac_val), sizeof(mac_addr_t) ) == 0 && \
		strncmp( (mac_entry_1->oif_name), (mac_entry_2->oif_name), IF_NAME_SIZE ) == 0 )


bool_t
mac_table_entry_add(mac_table_t *mac_table, mac_table_entry_t *mac_table_entry) {

	mac_table_entry_t *mac_table_entry_old = mac_table_lookup(mac_table, mac_table_entry->mac.mac_val);

	if(mac_table_entry_old && IS_MAC_TABLE_ENTRY_EQUAL(mac_table_entry_old, mac_table_entry)) { 
		return 0;	
	}
	
	if(mac_table_entry_old) {
		delete_mac_table_entry(mac_table, mac_table_entry_old->mac.mac_val);
	}

	init_glthread(&mac_table_entry->mac_entry_glue);
	glthread_add_next(&mac_table->mac_entries, &mac_table_entry->mac_entry_glue);
}


void
dump_mac_table(mac_table_t *mac_table) {

	glthread_t *curr;
	mac_table_entry_t *mac_table_entry;

	ITERATE_GLTHREAD_BEGIN(&mac_table->mac_entries, curr) {
		mac_table_entry = mac_entry_glue_to_mac_entry(curr);
		printf("\tMAC : %u:%u:%u:%u:%u:%u	| Intf : %s\n",
			mac_table_entry->mac.mac_val[0],
			mac_table_entry->mac.mac_val[1],
			mac_table_entry->mac.mac_val[2],
			mac_table_entry->mac.mac_val[3],
			mac_table_entry->mac.mac_val[4],
			mac_table_entry->mac.mac_val[5],
			mac_table_entry->oif_name);
	} ITERATE_GLTHREAD_END(&mac_table->mac_entries, curr);
}


/*
	API performans MAC learning
*/

static void
l2_switch_perform_mac_learning(node_t *node, char *src_mac, char *if_name) {

	if (mac_table_lookup(node->node_nw_props.mac_table, src_mac))
		return; 
	else { 
		
		mac_table_entry_t *mac_entry = calloc(1,sizeof(mac_table_entry_t));
		mac_entry->mac.mac_val[MAC_ADDR_SIZE] = '\0';
		strncpy(mac_entry->mac.mac_val, src_mac, MAC_ADDR_SIZE);
		strncpy(mac_entry->oif_name, if_name, IF_NAME_SIZE);
		mac_table_entry_add(node->node_nw_props.mac_table, mac_entry);
	}
	return;
}

static void
l2_switch_forward_frame(node_t *node, interface_t *recv_intf,
						char *pkt, unsigned int pkt_size) {
	/*If dst mac is broadcast mac, then flood the frame*/
	ethernet_hdr_t *ethernet_hdr = (ethernet_hdr_t *)pkt;
	if (IS_MAC_BROADCAST_ADDR(ethernet_hdr->dst_mac.mac_val)) {
		send_pkt_flood_l2_intf_only(node, recv_intf, pkt, pkt_size);
		return;
	}
	/* Check the mac table to forward the frame*/
	
	/*
	mac_table_entry_t *mac_table_entry = 
		mac_table_lookup(NODE_MAC_TABLE(node), ethernet_hdr->dst_mac.mac_val);

	if(!mac_table_entry) {
		send_pkt_flood_l2_intf_only(node, recv_intf, pkt, pkt_size);
		return;
	}

	char *oif_name = mac_table_entry->oif_name;
	interface_t *oif = get_node_if_by_name(node, oif_name);
	if(!oif) {
		return;
	}
	
	send_pkt_out(pkt, pkt_size, oif);
	*/	
}

void 
l2_switch_recv_frame(interface_t *interface, char *pkt, unsigned int pkt_size) {

	node_t *node = interface->att_node;
	printf("node name: %s\n", node->node_name);	
	ethernet_hdr_t *ethernet_hdr = (ethernet_hdr_t *)pkt;
	char *dst_mac = (char *)ethernet_hdr->dst_mac.mac_val;
	char *src_mac = (char *)ethernet_hdr->src_mac.mac_val;
	l2_switch_perform_mac_learning(node, src_mac, interface->if_name);
	l2_switch_forward_frame(node, interface, pkt, pkt_size);

}


















