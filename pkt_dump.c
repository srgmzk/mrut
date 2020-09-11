/*
 * =====================================================================================
 *
 *       Filename:  pkt_dump.c
 *
 *    Description
 *
 *        Version:  1.0
 *        Created:  08/31/2020 05:16:02 PM
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
#include "graph.h"
#include "Layer2/layer2.h"
#include "gluethread/glthread.h"


/*
	> Ethernet Hdr
	> ARP Hdr
	> IP Hdr
	> for unknown payload type (application data) - print the offset and size of payload in the frame


	typedef struct ehternet_hdr_{
		mac_addr_t dst_mac;
		mac_addr_t src_mac;
		unsigned short type;
		char payload[248]; //max allowed 1500
		unsigned int FCS;
	} ethernet_hdr_t;


	typedef struct arp_hdr_ {
		short hw_type;          // 1 for ehternet cable 
		short proto_type;       // 0x0800 for IPv4 
		char hw_addr_len;       // 6 for MAC 
		char proto_addr_len;    // 4 for IPv4 
		short op_code;          // req or reply 
		mac_addr_t src_mac;     // MAC of OIF interface 
		unsigned int src_ip;    // IP of OIF 
		mac_addr_t dst_mac;     // ? 
		unsigned int dst_ip;    // IP for which ARP is being resolved 
	} arp_hdr_t;

*/


void
pkt_dump(ethernet_hdr_t *ethernet_hdr, unsigned int pkt_size) {
	
	printf("PACKET DUMP:");
	printf("\n................................... ");
	printf("Ethernet dst mac: %s\n", ethernet_hdr->dst_mac.mac_val);
	printf("Ethernet src mac: %s\n", ethernet_hdr->src_mac.mac_val);
	printf("Pkt type:		  %d\n", ethernet_hdr->type);
	if( ethernet_hdr->type == 0x0806 ) { // ARP pkt
		printf("\t ARP PACKET DUMP");
		printf("\n\t....................................");
	} else if ( ethernet_hdr->type >= 0x0800 ) { // IP pkt
		printf("\n\t IP PACKET DUMP\n");
		printf("\n\t....................................");
	} else { 
		printf("Payload:		  %s\n", ethernet_hdr->payload);
	}
	printf("FCS: 			  %d\n", ethernet_hdr->FCS);
}
