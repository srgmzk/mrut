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

void 
apply_mask(char *prefix, char mask, char *str_prefix) {
	
	int bitmask = atoi(mask);
	
}

/*
 *	fill the 6-byte array passed as an argument with MAC broadcast address.
 */
void 
layer2_fill_with_broadcast_mac(char *mac_array) {

}


