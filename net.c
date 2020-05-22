/*
 * =====================================================================================
 *
 *       Filename:  net.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/19/2020 01:58:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "graph.h"
#include <memory.h>
#include "utils.h"
#include <stdio.h>
#include <assert.h>

/*Just some Random number generator*/
static unsigned int
hash_code(void *ptr, unsigned int size) {
	unsigned int value=0, i=0;
	char *str = (char*)ptr;
	while( i < size ) {
		value += *str;
		value *= 97;
		str++;
		i++;
	}
	return value;
} 

/*Heuristic, Assign a unique mac address to interface*/
void
interface_assign_mac_address(interface_t *interface) {
	
	node_t *node = interface->att_node;
	//char mac[18] = "00:11:22:33:44:55";	
	if(!node)
		return;

	unsigned int hash_code_val = 0;
	hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE);
	hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE);
	memset(IF_MAC(interface), 0, sizeof(IF_MAC(interface)));
	memcpy(IF_MAC(interface), (char *)&hash_code_val, sizeof(unsigned int));
	//memcpy(IF_MAC(interface), mac, 18);
	//printf("interface %s MAC %s \n", interface->if_name, IF_MAC(interface));//, (char *)&hash_code_val);
}

bool_t node_set_loopback_address(node_t *node, char *ip_addr) {
	
	assert(ip_addr);
	node->node_nw_props.is_lb_addr_config = true;
	strncpy(NODE_LO_ADDR(node), ip_addr, 16);
	NODE_LO_ADDR(node)[15] = '\0';
	
	return true;
}


bool_t node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask) {
	
	interface_t *interface = get_node_if_by_name(node, local_if);
	if(!interface) assert(0);

	strncpy(IF_IP(interface), ip_addr, 16);	
		
	IF_IP(interface)[15] = '\0';
	interface->intf_nw_props.mask = mask;
	interface->intf_nw_props.is_ipaddr_config = true;
	return true;
}

bool_t node_unset_intf_ip_address(node_t *node, char *local_if) {
	return true;
}

unsigned int convert_ip_from_str_to_int(char *ip_addr) {
	unsigned buf[4] = {0,0,0,0};
	/* (first octet * 16777216) + (second octet * 65536) + (third octet * 256) + (fourth octet) */
	int ret = sscanf(in, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]); 
	if (out) 
		return (buf[0]*16777216L + buf[1]*65536L + buf[2]*256L + buf[3]);
	return 1;

}

void convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer) {

}


