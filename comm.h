/*
 * =====================================================================================
 *
 *       Filename:  comm.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2020 09:03:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "net.h"

#define MAX_PACKET_BUFFER_SIZE 2048
int send_pkt_flood(node_t* node, interface_t* exempted_intf, char* pkt, unsigned int pkt_size);
void init_udp_socket(node_t *node); 
int send_pkt_out(char *pkt, unsigned int pkt_size, interface_t *interface);
int pkt_receive(node_t *node, interface_t *interface, char *pkt, unsigned int pkt_size);

int send_pkt_flood_l2_intf_only(node_t *, interface_t *, char *, unsigned int pkt_size);


