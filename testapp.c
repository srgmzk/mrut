#include "graph.h"
#include <stdio.h>
#include "utils.h"

extern graph_t *build_first_topo();


int main(int argc, char **argv) {
	
	graph_t *topo = build_first_topo();
	node_t *node = NULL;
	interface_t *iface = NULL;
	node_t *nbrNode =  NULL;

	node = get_node_by_node_name(topo, "R0_re");
	iface = get_node_if_by_name(node, "eth0/0");
	interface_assign_mac_address(iface);
//	nbrNode = get_nbr_node(iface);
	iface = get_node_if_by_name(node, "eth0/4");
	interface_assign_mac_address(iface);
	
	node = get_node_by_node_name(topo, "R1_re");
	iface = get_node_if_by_name(node, "eth0/1");
	interface_assign_mac_address(iface);
	iface = get_node_if_by_name(node, "eth0/2");
	interface_assign_mac_address(iface);

	node = get_node_by_node_name(topo, "R2_re");
	iface = get_node_if_by_name(node, "eth0/3");
	interface_assign_mac_address(iface);
	iface = get_node_if_by_name(node, "eth0/5");
	interface_assign_mac_address(iface);

/*
	char *in = "255.255.255.255";
	unsigned char mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	char mac2[6] = {0x02,0xff,0xff,0xff,0xff,0xff};
	char mask = 32;
	
	unsigned long out;
	char output_buffer[16] = {"\0"};
	char output_prefix[16] = {"\0"};

	out = convert_ip_from_str_to_int(in);
	printf("Convert IP from str %s: %ld\n", in, out);
	
	convert_ip_from_int_to_str(out, output_buffer);
	printf("Convert IP form int %ld: %s\n", out, output_buffer);	
	
	apply_mask(output_buffer, mask, output_prefix);
	printf("IP with applied %d mask: %s\n", mask, output_prefix);	
	
	printf("mac brcast ? ... %s\n", (IS_MAC_BROADCAST_ADDR(mac)) ? "yes" : "no");		
	printf("mac2 brcast ? ... %s\n", (IS_MAC_BROADCAST_ADDR(mac2)) ? "yes" : "no");		
	layer2_fill_with_broadcast_mac(mac2);
	printf("mac2 brcast after fill ? ... %s\n", (IS_MAC_BROADCAST_ADDR(mac2)) ? "yes" : "no");		

	
	node = get_node_by_node_name(topo, "R0_re");
	iface = node_get_matching_subnet_interface(node, "30.1.1.2" );
	if (iface) {

		printf("\nFound by IP Interface %s properties:\n",  iface->if_name);
		printf(" - %s: %s \n", "mac addr", IF_MAC(iface));
		printf(" - %s: %s \n", "ip addr", IF_IP(iface));
		printf(" - %s: %s \n", "is ip interface?", IS_INTF_L3_MODE(iface) ? "yes": "no");
	} else {
		printf("Iface not found\n");
	}
*/


	dump_nw_graph(topo);
	return 0;

}
