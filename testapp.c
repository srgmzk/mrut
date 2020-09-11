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
extern graph_t *build_first_topo();
extern graph_t *build_simple_l2_switch_topo();
extern void nw_init_cli();

graph_t *topo = NULL;

int 
main(int argc, char** argv) {

	nw_init_cli();
	topo = build_simple_l2_switch_topo();
	//topo = build_first_topo();

	node_t *node = NULL;
	interface_t *iface = NULL;
	
	node = get_node_by_node_name(topo, "H1");
	iface = get_node_if_by_name(node, "eth0/5");
	interface_assign_mac_address(iface);
 	node = get_node_by_node_name(topo, "H2");
	iface = get_node_if_by_name(node, "eth0/8");
	interface_assign_mac_address(iface);
	node = get_node_by_node_name(topo, "H3");
	iface = get_node_if_by_name(node, "eth0/6");
	interface_assign_mac_address(iface);
	node = get_node_by_node_name(topo, "H4");
	iface = get_node_if_by_name(node, "eth0/7");
	interface_assign_mac_address(iface);

	char msg[] = "HelloPkt";
	ethernet_hdr_t* l2hdr;

	//printf("ETH HDR SIZE: %d\n", ETH_HDR_SIZE_EXCL_PAYLOAD);

	l2hdr = ALLOC_ETH_HDR_WITH_PAYLOAD(msg, 8);


	//printf("ETH_FCS: %d\n", ETH_FCS(l2hdr, 8));
	
	//extern void
	//pkt_dump(ethernet_hdr_t *ethernet_hdr, unsigned int pkt_size);
	

//	char msg[] = "Hello, how are you\0";
//	send_pkt_out(msg, strlen(msg), iface);	
//	send_pkt_flood(node, iface, msg, strlen(msg));

	node = get_node_by_node_name(topo, "L2SW");
	iface = get_node_if_by_name(node, "eth0/1");

	send_pkt_flood_l2_intf_only(node, iface, msg, strlen(msg));


//	start_shell();
	return 0;
}
