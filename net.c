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
#include <stdlib.h>



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
	//printf("value: %d \n", (unsigned int)value);
	return value;
} 

/*Heuristic, Assign a unique mac address to interface*/
void
interface_assign_mac_address(interface_t *interface) {
	
	node_t *node = interface->att_node;
	
	if(!node)
		return;

	unsigned int hash_code_val = 0;//,  hash_code_val2 = 0;
	char oct1[7] = "\0";

	char oct2[7] = "\0";
	hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE);

	snprintf(oct1, 7, "%x", hash_code_val);
	
	
	hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE);

	snprintf(oct2, 7, "%x", hash_code_val);

//	printf("oct1 %s oct2 %s\n", oct1, oct2);
	sprintf(IF_MAC(interface), "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c\0", 
	'0','0', oct1[0],oct1[1], oct1[2],oct1[3], oct1[4],oct1[5], oct2[0],oct2[1], oct2[2],oct2[3]);
//	printf("interface %s MAC %s\n", interface->if_name, IF_MAC(interface));

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


/*
	converts an ip address in integer form into its equivalent dot decimal form (A.B.C.D format) 
*/
void
convert_ip_from_int_to_str(unsigned int val, char *output_buffer) {

	unsigned buf[4] = {0,0,0,0};
	char bbuf[33] = "\0";

	int2bstr(val, bbuf, 33);
	ip2oct(bbuf, buf);
	OCT2IP(buf, output_buffer);
	
}

/*
	converts an ip address in string form (A.B.C.D format) into its equivalent 32-bit integer form.
*/

unsigned int convert_ip_from_str_to_int(char *ip_addr) {
	unsigned buf[4] = {0,0,0,0};
	int ret = sscanf(ip_addr, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]); 
	return (ret) ? SUMIP(buf) : 1;
}


interface_t *
node_get_matching_subnet_interface(node_t *node, char *ip_addr) {

	int i = 0;
	char node_addr[16] = "\0";
	char req_addr[16] = "\0";
	while ( node->intf[i]) {
 		if ( node->intf[i]->intf_nw_props.is_ipaddr_config ) {	
			apply_mask(	IF_IP(node->intf[i]), \
						node->intf[i]->intf_nw_props.mask, \
						node_addr);
			apply_mask(	ip_addr, \
						node->intf[i]->intf_nw_props.mask, \
						req_addr);
			
			if ( strncmp(node_addr, req_addr, 16) == 0)
				return (node->intf[i]);
		}
		i++;
	}


	return NULL;
}


//GLTHREAD_TO_STRUCT(gthread_to_node, node_t, glthread/*, glthreadptr*/);

void 
dump_nw_graph(graph_t *graph) {
	node_t *node;
	int i;

	glthread_t node_list = graph->node_list;
    glthread_t *curr = NULL;
    ITERATE_GLTHREAD_BEGIN(&node_list, curr){
		node = glthread_to_node(curr); 
        printf(" > Node: %s\n", node->node_name);
		i = 0;
		while ( node->intf[i]) {
			printf("\t* Interface %s (neighbor: %s )\n", IF_NAME(node->intf[i]), \
														 get_nbr_node(node->intf[i])->node_name);
			printf("\t\tmac: %s\n", IF_MAC(node->intf[i]));
			printf("\t\tip: %s/%d\n", IF_IP(node->intf[i]), \
								node->intf[i]->intf_nw_props.mask);
			printf("\t\tloopback: %s\n", NODE_LO_ADDR(node));
			i++;
			
		}
    } ITERATE_GLTHREAD_END(&base_glthread, curr);
 
	return; 
}



void *
pkt_buffer_shift_right(char* pkt, int pkt_size, int empty_size) {

	char *ret, *pkt_buf;
	
	pkt_buf = malloc(pkt_size*(sizeof(char)));
	memcpy(pkt_buf, pkt, pkt_size);
	ret = memset(pkt, empty_size, 0);
	ret += empty_size;
	memcpy(ret, pkt_buf, pkt_size);
	free(pkt_buf);
	return ret;

}
