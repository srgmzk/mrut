/*
 * =====================================================================================
 *
 *       Filename:  testApp2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/06/2020 11:24:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "graph.h" 
#include "Layer2/layer2.h"


extern graph_t *build_square_topo();
extern graph_t *build_linear_topo();
extern void nw_init_cli();

graph_t *topo = NULL;

int 
main(int argc, char** argv) {

	nw_init_cli();
	topo = build_linear_topo();

	node_t *node = NULL;
	interface_t *iface = NULL;


	node = get_node_by_node_name(topo, "H1");
	iface = get_node_if_by_name(node, "eth0/1");
	interface_assign_mac_address(iface);

	char msg[] = "HelloPkt";
	ethernet_hdr_t* l2hdr;

	printf("ETH HDR SIZE: %d\n", ETH_HDR_SIZE_EXCL_PAYLOAD);

	l2hdr = ALLOC_ETH_HDR_WITH_PAYLOAD(msg, 8);


	printf("ETH_FCS: %d\n", ETH_FCS(l2hdr, 8));


//	start_shell();
	return 0;
}
