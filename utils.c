/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/21/2020 06:01:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  srg mos 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "utils.h"
/*
 * prefix - is a pointer to string representing an IP address.
 * mask - is any value [0-32]
 * str_prefix - pointer to a char array of size 16 bytes. This function must fill this array 
 * with the output.
 * 
 * This function must apply the mask on prefix and store the result in str_prefix.
 * 
 * For eg : prefix = 122.1.1.1, mask 24, then str_prefix will store 122.1.1.0
 *
*/
unsigned long convert_ip_from_str_to_int(char* prefix); 

void 
ip2oct(char* in, unsigned* out) { 									
	int i = 0;											
	char oct[8] = {"\0"};								
	while (i < 4) {										
		memcpy(oct, &in[i*8], 8);						
		//oct[8] = '\0';									
		out[i] = (unsigned int)strtol(oct,NULL,2);  
		i++;
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


void
apply_mask(char *prefix, char mask, char *str_prefix) {
	unsigned msk = (unsigned char)(mask);
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
 *	fill the 6-byte array passed as an argument with MAC broadcast address.
 */

void
layer2_fill_with_broadcast_mac(char *mac) {
	mac[0] = 0xff;
	mac[1] = 0xff;
	mac[2] = 0xff;
	mac[3] = 0xff;
	mac[4] = 0xff;
	mac[5] = 0xff;
}





