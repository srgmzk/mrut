/*
 * =====================================================================================
 *
 *       Filename:  layer2.h
 *
 *    Description:  This file define the structures required for layer 2 functionality
 *
 *        Version:  1.0
 *        Created:  07/06/2020 12:41:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef __LAYER2__
#define __LAYER2__

#include "../net.h"
#include "../gluethread/glthread.h"
#include <string.h>
#include <stdlib.h>


#pragma pack (push, 1) /* now compiler do not insert any padding bytes in between the members of these data */
typedef struct ehternet_hdr_{
	mac_addr_t dst_mac;
	mac_addr_t src_mac;
	unsigned short type;
	char payload[248]; /* max allowed 1500 */
	unsigned int FCS;
} ethernet_hdr_t;
#pragma pack(pop)

#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t*)0)->payload))

//#define ETH_FCS(eth_hdr_ptr, payload_size) (eth_hdr_ptr->FCS) // -- ?

#define ETH_FCS(eth_hdr_ptr, payload_size)  \
    (*(unsigned int *)(((char *)(((ethernet_hdr_t *)eth_hdr_ptr)->payload) + payload_size)))

static inline ethernet_hdr_t *
ALLOC_ETH_HDR_WITH_PAYLOAD(char *pkt, unsigned int pkt_size) {
	ethernet_hdr_t* ether_hdr;
	ether_hdr = calloc(1, sizeof(ethernet_hdr_t));
	memcpy(ether_hdr->payload, pkt, pkt_size);
	return ether_hdr;	
}

#endif

