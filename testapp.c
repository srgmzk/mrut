#include "graph.h"

extern graph_t *build_first_topo();


int main(int argc, char **argv) {
	
	graph_t *topo = build_first_topo();
	node_t *node = get_node_by_node_name(topo, "R0_re");
	node_t *nbrNode;
	interface_t *iface = get_node_if_by_name(node, "eth0/0"); 
	nbrNode = get_nbr_node(iface);
	printf("Node: %s  NbrNode: %s \n", node->node_name, nbrNode->node_name);
	
	//dump_graph(topo);
	return 0;

}
