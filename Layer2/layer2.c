/*
 * =====================================================================================
 *
 *       Filename:  layer2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/06/2020 02:18:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "layer2.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>


/* A Routine to resolve ARP out of oif */
void
send_arp_broadcast_request(node_t *node, interface_t *oif, char *ip_addr) {
	
	/*Take memory which can accomodate Ethernet hdr + ARP hdr*/
	unsigned int payload_size = sizeof(arp_hdr_t);
	ethernet_hdr_t *ethernet_hdr = (ethernet_hdr_t *)calloc(1, 
				ETH_HDR_SIZE_EXCL_PAYLOAD + payload_size);
	if(!oif) {
		oif = node_get_matching_subnet_interface(node, ip_addr);
		if(!oif) {
			printf("Error: %s : No eligible subnet for ARP resolution for IP-address : %s", 
				node->node_name, ip_addr);
			return;
		}
	}
	/* STEP 1: Prepare ethernet hdr */	
	layer2_fill_with_broadcast_mac(ethernet_hdr->dst_mac.mac_val);
	memcpy(ethernet_hdr->src_mac.mac_val, IF_MAC(oif), sizeof(mac_addr_t));
	ethernet_hdr->type = ARP_MSG;

	/* STEP 2: Prepare ARP Broadcast Request Msg out of oif */
	arp_hdr_t *arp_hdr = (arp_hdr_t *)ethernet_hdr->payload;
	arp_hdr->hw_type = 1;
	arp_hdr->proto_type = 0x0800;
	arp_hdr->hw_addr_len = sizeof(mac_addr_t);
	arp_hdr->proto_addr_len = 4;
	arp_hdr->op_code = ARP_BROAD_REQ;

	memcpy(arp_hdr->src_mac.mac_val, IF_MAC(oif), sizeof(mac_addr_t));

	inet_pton(AF_INET, IF_IP(oif), &arp_hdr->src_ip);
	arp_hdr->src_ip = htonl(arp_hdr->src_ip);

	memset(arp_hdr->dst_mac.mac_val, 0, sizeof(mac_addr_t));

	inet_pton(AF_INET, ip_addr, &arp_hdr->dst_ip);
	arp_hdr->dst_ip = htonl(arp_hdr->dst_ip); 
	
	/* Never use ehternet_hdr->FCS = 0, because FCS lies at the 
	 	end of payload, and not at the end of ethernet header !! */
	ETH_FCS(ethernet_hdr, sizeof(arp_hdr_t)) = 0; /* Not used */

	/*STEP 3: Now dispatch the ARP Broadcast Request Packet out of interface */
	send_pkt_out((char *)ethernet_hdr, ETH_HDR_SIZE_EXCL_PAYLOAD + payload_size, oif);

	free(ethernet_hdr);
}

static void
send_arp_reply_msg(ethernet_hdr_t *ethernet_hdr_in, interface_t *oif) {

		
	arp_hdr_t *arp_hdr_in = (arp_hdr_t *)(GET_ETHERNET_HDR_PAYLOAD(ethernet_hdr_in));	
	ethernet_hdr_t *ethernet_hdr_reply = (ethernet_hdr_t *)calloc(1, MAX_PACKET_BUFFER_SIZE);

	memcpy(ethernet_hdr_reply->dst_mac.mac_val, arp_hdr_in->src_mac.mac_val, sizeof(mac_addr_t));
	memcpy(ethernet_hdr_reply->src_mac.mac_val, IF_MAC(oif), sizeof(mac_addr_t));
	
	ethernet_hdr_reply->type = ARP_MSG;
	arp_hdr_t *arp_hdr_reply = (arp_hdr_t *)(GET_ETHERNET_HDR_PAYLOAD(ethernet_hdr_reply));

	arp_hdr_reply->hw_type = 1;
	arp_hdr_reply->proto_type = 0x0800;
	arp_hdr_reply->hw_addr_len = sizeof(mac_addr_t);
	arp_hdr_reply->proto_addr_len = 4;
	
	arp_hdr_reply->op_code = ARP_REPLY;
	memcpy(arp_hdr_reply->src_mac.mac_val, IF_MAC(oif), sizeof(mac_addr_t));
	
	inet_pton(AF_INET, IF_IP(oif), &arp_hdr_reply->src_ip);
	arp_hdr_reply->src_ip = htonl(arp_hdr_reply->src_ip);

	memcpy(arp_hdr_reply->dst_mac.mac_val, arp_hdr_in->src_mac.mac_val, sizeof(mac_addr_t));
	arp_hdr_reply->dst_ip = arp_hdr_in->src_ip;

	ETH_FCS(ethernet_hdr_reply, sizeof(arp_hdr_t)) = 0;

	unsigned int total_pkt_size = ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t);

	char *shifted_pkt_buffer = pkt_buffer_shift_right((char *)ethernet_hdr_reply,
								total_pkt_size, MAX_PACKET_BUFFER_SIZE);
	send_pkt_out(shifted_pkt_buffer, total_pkt_size, oif);
	free(ethernet_hdr_reply);

}

static void 
process_arp_reply_msg(node_t *node, interface_t *iif, ethernet_hdr_t *ethernet_hdr) {
	
	printf("%s : ARP reply msg recvd on interface %s of node %s\n", 
			__FUNCTION__, iif->if_name, iif->att_node->node_name);
	arp_table_update_from_arp_reply( NODE_ARP_TABLE(node), 
			(arp_hdr_t *)GET_ETHERNET_HDR_PAYLOAD(ethernet_hdr), iif);
}

static void 
process_arp_broadcast_request(node_t *node, interface_t *iif, ethernet_hdr_t *ethernet_hdr) {
	printf("%s : ARP Broadcast msg recvd on interface %s of node %s\n",
			__FUNCTION__, iif->if_name, iif->att_node->node_name);
	char ip_addr[16];
	arp_hdr_t *arp_hdr = (arp_hdr_t *)(GET_ETHERNET_HDR_PAYLOAD(ethernet_hdr));

	unsigned int arp_dst_ip = htonl(arp_hdr->dst_ip);
	inet_ntop(AF_INET, &arp_dst_ip, ip_addr, 16);
	ip_addr[15] = '\0';
	
	if(strncmp(IF_IP(iif), ip_addr, 16)) {
		printf("%s : ARP Broadcast req msg dropped, Dst IP address %s did not match with interface ip: %s\n",
				node->node_name, ip_addr, IF_IP(iif));
		return;
	}
	send_arp_reply_msg(ethernet_hdr, iif);
	
}

static inline bool_t
l2_framee_recv_qualify_on_interface(interface_t *interface, ethernet_hdr_t *ethernet_hdr) {
	/* Presence of IP address on interface makes it work in L3 mode, 
	* while absence of IP-address automatically make it work in 
	* L2 mode. For interfaces working in L2 mode (L2 switch interface),
	* we should accept all frames. L2 switch never discard the frames 
	* based on MAC addresses.	
	*/

	if(interface->intf_nw_props.is_ipaddr_config)
		return 0; // < bug fix
	/* Return TRUE if receiving machine must accept the frame */

	if(memcmp(IF_MAC(interface),
		ethernet_hdr->dst_mac.mac_val,
		sizeof(mac_addr_t)) == 0) {
			return 1;
		}
	if(IS_MAC_BROADCAST_ADDR(ethernet_hdr->dst_mac.mac_val)) {
		return 1;
	}	
	return 0;
}

extern void 
l2_switch_recv_frame(interface_t *interface,
						char *pkt, unsigned int pkt_size);
void
layer2_frame_recv(node_t *node, interface_t *interface,
				char *pkt, unsigned int pkt_size) {
	
	/* Entry point into TCP/IP stack from bottom */

	ethernet_hdr_t *ethernet_hdr = (ethernet_hdr_t *)pkt;
	if ( l2_frame_recv_qualify_on_interface(interface, ethernet_hdr) == 0 ) {
		printf("L2 Frame Rejected");
		return;
	}
	
	printf("L2 Frame Accepted\n");	
		if ( IS_INTF_L3_MODE(interface) ) {
			switch(ethernet_hdr->type) {
				case ARP_MSG:
				{
					arp_hdr_t *arp_hdr = (arp_hdr_t *)( ethernet_hdr->payload );
					switch(arp_hdr->op_code) {
						case ARP_BROAD_REQ:
							process_arp_broadcast_request(node, interface, ethernet_hdr);
						break;
						case ARP_REPLY:
							process_arp_reply_msg(node, interface, ethernet_hdr);			
						break;
					}
			
				}
				break;
				default: 
					//promote_pkt_to_layer3(node, interface, pkt, pkt_size);
					break;
			}
		} 
		else if( IF_L2_MODE(interface) == ACCESS || IF_L2_MODE(interface) == TRUNK ) {
			l2_switch_recv_frame(interface, pkt, pkt_size);
			
		}

}

void
init_arp_table(arp_table_t **arp_table) {
	*arp_table = calloc(1, sizeof(arp_table_t));
	init_glthread(&((*arp_table)->arp_entries));
}

arp_entry_t *
arp_table_lookup(arp_table_t *arp_table, char *ip_addr) {
	glthread_t *curr;
	arp_entry_t *arp_entry;
	ITERATE_GLTHREAD_BEGIN(&arp_table->arp_entries, curr) {
		if(strncmp(arp_entry->ip_addr.ip_val, ip_addr, 16) == 0) {
			return arp_entry;
		}
		
	} ITERATE_GLTHREAD_END(&arp_table->arp_entries, curr)
	return NULL;
}


void 
delete_arp_table_entry(arp_table_t *arp_table, char *ip_addr) {

}

bool_t
arp_table_entry_add(arp_table_t *arp_table, arp_entry_t *arp_entry) {

	arp_entry_t *arp_entry_old = arp_table_lookup(arp_table, arp_entry->ip_addr.ip_val);
	if (arp_entry_old && memcmp(arp_entry_old, arp_entry, sizeof(arp_entry_t)) == 0)
		return 0;		
	if (arp_entry_old) {
		delete_arp_table_entry(arp_table, arp_entry->ip_addr.ip_val);
	}
	init_glthread(&arp_entry->arp_glue);
	glthread_add_next(&arp_table->arp_entries, &arp_entry->arp_glue);
	return 1;
}

void
arp_table_update_from_arp_reply(arp_table_t *arp_table, arp_hdr_t *arp_hdr, interface_t *iif) {
	unsigned int src_ip = 0;
	assert(arp_hdr->op_code == ARP_REPLY);
	arp_entry_t *arp_entry = calloc(1,sizeof(arp_entry_t));
	src_ip = htonl(arp_hdr->src_ip);
	inet_ntop(AF_INET, &src_ip, arp_entry->ip_addr.ip_val, 16);
	arp_entry->ip_addr.ip_val[15] = '\0';
	memcpy(arp_entry->mac_addr.mac_val, arp_hdr->src_mac.mac_val, sizeof(mac_addr_t));
	strncpy(arp_entry->oif_name, iif->if_name, IF_NAME_SIZE);
	
	bool_t rc = arp_table_entry_add(arp_table, arp_entry);
	if(rc == 0){
		free(arp_entry);
	}
}

void
dump_arp_table(arp_table_t *arp_table){

	glthread_t *curr;
	arp_entry_t *arp_entry;
	ITERATE_GLTHREAD_BEGIN(&arp_table->arp_entries, curr) {
		arp_entry = arp_glue_to_arp_entry(curr);
		printf("IP : %s, MAC : %u:%u:%u:%u:%u:%u, OIF = %s\n",
			arp_entry->ip_addr.ip_val,
			arp_entry->mac_addr.mac_val[0],
			arp_entry->mac_addr.mac_val[1],
			arp_entry->mac_addr.mac_val[2],
			arp_entry->mac_addr.mac_val[3],
			arp_entry->mac_addr.mac_val[4],
			arp_entry->mac_addr.mac_val[5],
			arp_entry->oif_name);
		} ITERATE_GLTHREAD_END(&arp_table->arp_entries, curr);
}


void 
interface_set_l2_mode(node_t *node, interface_t *intf, /* char* */intf_l2_mode_t mode) {

	intf->intf_nw_props.intf_l2_mode = mode;
}


/*APIs to be used to create topology*/
void
node_set_intf_l2_mode(node_t *node, char *intf_name,
						intf_l2_mode_t intf_l2_mode) {
	
	interface_t *interface = get_node_if_by_name(node, intf_name);
	assert(interface);
	interface_set_l2_mode(node, interface, intf_l2_mode /*intf_l2_mode_str(intf_l2_mode)*/);
}


