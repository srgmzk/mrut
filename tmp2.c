/*
 * =====================================================================================
 *
 *       Filename:  tmp2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/03/2020 02:32:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>

/* VLAN support */
#pragma pack(push, 1)
/* Vlan 802.1q 4 byte hdr */
typedef struct ehternet_hdr_{
	char dst_mac[8];
	char src_mac[8];
	unsigned short type;
	char payload[248]; /* max allowed 1500 */
	unsigned int FCS;
} ethernet_hdr_t;


typedef struct vlan_8021q_hdr_{
	unsigned short tpid; /* = 0x8100 */
	short tci_pcp : 3;	 /* initial 4 bits not used in this course */
	short tci_dei : 1;	 /* not used */
	short tci_vid : 12;	 /* Tagged vlan id */
} vlan_8021q_hdr_t;

typedef struct vlan_ethernet_hdr_{
	char dst_mac[8];
	char src_mac[8];
	vlan_8021q_hdr_t vlan_8021q_hdr;
	unsigned short type;
	char payload[248]; /* max allowed 1500 */
	unsigned int FCS;
} vlan_ethernet_hdr_t;
#pragma pack(pop)

/* Return NULL if Input ethernet hdr is not vlan tagged, else return pointer to 801.1q vlan hdr 
embedded in ethernet hdr*/

static inline vlan_8021q_hdr_t *
is_pkt_vlan_tagged(ethernet_hdr_t* ethernet_hdr) {
	vlan_ethernet_hdr_t* vlan_eth_hdr;
	if (ethernet_hdr->type != 0x8100)	
		return NULL;
	vlan_eth_hdr = (vlan_ethernet_hdr_t *)ethernet_hdr;
	return  &(vlan_eth_hdr->vlan_8021q_hdr);
}

int main() {


 ethernet_hdr_t hdr;
 hdr.dst_mac[8] = "AAAAAAA\0";
 hdr.src_mac[8] = "BBBBBBB\0";
 hdr.type = 0x8100;
 hdr.payload[248] = "9000";
 hdr.FCS = 5;


 vlan_8021q_hdr_t *vlan_hdr;
 vlan_hdr = is_pkt_vlan_tagged(&hdr);
 
 printf("vlan hdr: %p\n", vlan_hdr);

 return 0;

}
