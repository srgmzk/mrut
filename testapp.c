#include "graph.h"
#include <stdio.h>
extern graph_t *build_first_topo();


int main(int argc, char **argv) {
	
	graph_t *topo = build_first_topo();

	node_t *node = get_node_by_node_name(topo, "R0_re");
	node_t *nbrNode;
	interface_t *iface = get_node_if_by_name(node, "eth0/0");

	interface_assign_mac_address(iface);
	nbrNode = get_nbr_node(iface);

	printf("Node %s properties: \n", node->node_name);
	printf(" - %s: %s \n", "neighbour", nbrNode->node_name);
	printf(" - %s: %s \n", "is loopback addr config?", ((node->node_nw_props.is_lb_addr_config) ? "yes": "no"));
	printf(" - %s: %s \n", "loopback addr", node->node_nw_props.lb_addr.ip_val);
	
	printf("\nInterface %s properties:\n",  iface->if_name);
	printf(" - %s: %s \n", "mac addr", IF_MAC(iface));
	printf(" - %s: %s \n", "ip addr", IF_IP(iface));
	printf(" - %s: %s \n", "is ip interface?", IS_INTF_L3_MODE(iface) ? "yes": "no");
	//dump_graph(topo);
	return 0;

}
