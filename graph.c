/*
 * =====================================================================================
 *
 *       Filename:  graph.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/13/2020 07:58:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "graph.h"
#include "comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void 
insert_link_between_two_nodes(node_t *node1,
		node_t *node2,
		char *from_if_name,
		char *to_if_name,
		unsigned int cost) {

	link_t *link = calloc(1, sizeof(link_t));
	
	int empty_intf_slot1 = 0;
	int empty_intf_slot2 = 0;

	/* Set interface properties */
	strncpy(link->intf1.if_name, from_if_name, IF_NAME_SIZE);
	link->intf1.if_name[IF_NAME_SIZE] = '\0';
	strncpy(link->intf2.if_name, to_if_name, IF_NAME_SIZE);
	link->intf2.if_name[IF_NAME_SIZE] = '\0';

	link->intf1.link = link; /*set back pointer to link*/
	link->intf2.link = link; /*set back pointer to link*/
	
	link->intf1.att_node = node1;
	link->intf2.att_node = node2;
	link->cost = cost;
	
	/*Plugin interface ends into Node*/
	empty_intf_slot1 = get_node_intf_available_slot(node1);
	node1->intf[empty_intf_slot1] = &link->intf1;
	init_intf_nw_props(&(node1->intf[empty_intf_slot1]->intf_nw_props));	

	empty_intf_slot2 = get_node_intf_available_slot(node2);
	node2->intf[empty_intf_slot2] = &link->intf2;
	init_intf_nw_props(&(node2->intf[empty_intf_slot2]->intf_nw_props));

}

graph_t *
create_new_graph(char *topology_name) {
	
	graph_t *graph = calloc(1, sizeof(graph_t));
	strncpy(graph->topology_name, topology_name, 32);
	graph->topology_name[32] = '\0';

	init_glthread(&graph->node_list);
	return graph;
	
}

node_t *
create_graph_node(graph_t *graph, char *node_name) {
	node_t *node = calloc(1, sizeof(node_t));
	strncpy(node->node_name, node_name, NODE_NAME_SIZE);
	node->node_name[NODE_NAME_SIZE] = '\0';

	init_udp_socket(node);

	init_node_nw_props(&(node->node_nw_props));
	init_glthread(&node->graph_glue);
	glthread_add_next(&graph->node_list, &node->graph_glue);
	return node;
} 


int 
node_name_cmp( void* node_name1, void* node_name2) {
	
	char *nn1 = (char *)node_name1;
	char *nn2 = (char *)node_name2;
	
	if (strcmp(nn1, nn2)) 
		return 0;
	return 1;
}






