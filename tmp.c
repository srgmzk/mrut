/*
 * =====================================================================================
 *
 *       Filename:  tmp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/21/2020 08:12:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "graph.h"

#define IS_MAC_BROADCAST_ADDR(mac) \
	(((unsigned char)(mac[0]) &	\
		(unsigned char)(mac[1]) &	\
		(unsigned char)(mac[2]) &	\
		(unsigned char)(mac[3]) &	\
		(unsigned char)(mac[4]) &	\
		(unsigned char)(mac[5]) ) == 0xff) ? 1 : 0 \
		

#define CHKBIT(val) ( (val) ? '1':'0' )
#define SUMIP(arr) (((arr[0])*16777216L) + ((arr[1])*65536L) + ((arr[2]) * 256L) + (arr[3]))
#define OCT2IP(in, out) sprintf(out, "%d.%d.%d.%d", (in)[0],(in)[1],(in)[2],(in)[3])

 
void 
ip2oct(char* in, unsigned* out) { 									
	int i = 0;											
	char oct[8] = {"\0"};								
	while (i < 4) {										
		memcpy(oct, &in[i*8], 8);						
		//oct[8] = '\0';									
		out[i] = (unsigned int)strtol(oct,NULL,2);  
		i++;
		//printf("%d\n", i);	
	}											
}													


void 
int2bstr(unsigned int val, char* bbuf, unsigned int size) {
	
	int i = size - 2, tmp = 0;
	while ( i ) {
		tmp = val % 2;
		bbuf[i] = CHKBIT(tmp); 
		--i;
		val /= 2;
	}
	bbuf[i] = CHKBIT(val);
	bbuf[size] = '\0'; 
}
	

/*
	converts an ip address in integer form into its equivalent dot decimal form (A.B.C.D format) 
*/
void
convert_ip_from_int_to_str(unsigned int val, char *output_buffer) {

	unsigned buf[4] = {0,0,0,0};
	char bbuf[33] = "\0";

	int2bstr(val, bbuf, 33);
	ip2oct(bbuf, buf);
	OCT2IP(buf, output_buffer);
	
}

/*
	converts an ip address in string form (A.B.C.D format) into its equivalent 32-bit integer form.
*/

unsigned int convert_ip_from_str_to_int(char *ip_addr) {
	unsigned buf[4] = {0,0,0,0};
	int ret = sscanf(ip_addr, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]); 
	return (ret) ? SUMIP(buf) : 1;
}


void
apply_mask(char *prefix, char mask, char *str_prefix) {
	unsigned msk = (unsigned char)mask;
	unsigned buf[4] = {0,0,0,0};
	char bbuf[33];
	int i = 0;

	unsigned long val = convert_ip_from_str_to_int(prefix); 

	int2bstr(val, bbuf, 33);
	if (msk > 0)
		for (i = msk; i < 33; ++i)
			bbuf[i] = '0';
	bbuf[33] = '\0';
	
	ip2oct(bbuf, buf);
	OCT2IP(buf, str_prefix);
}

/*
interface_t *
node_get_matching_subnet_interface(node_t *node, char *ip_addr) {

	
	return 0;

}
*/
void
layer2_fill_with_bradcast_nac(char *mac) {
	mac[0] = 0xff;
	mac[1] = 0xff;
	mac[2] = 0xff;
	mac[3] = 0xff;
	mac[4] = 0xff;
	mac[5] = 0xff;
}

int main(void) {

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
	layer2_fill_with_bradcast_nac(mac2);
	printf("mac2 brcast after fill ? ... %s\n", (IS_MAC_BROADCAST_ADDR(mac2)) ? "yes" : "no");		
/*
	node_t *node;
	node_get_matching_subnet_interface(node, in);
*/

	return 0;
}
