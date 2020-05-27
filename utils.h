/*
 * =====================================================================================
 *
 *       Filename:  utils.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/21/2020 06:00:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  srg mos, 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define IS_MAC_BROADCAST_ADDR(mac) \
	(((unsigned char)(mac[0]) &	\
		(unsigned char)(mac[1]) &	\
		(unsigned char)(mac[2]) &	\
		(unsigned char)(mac[3]) &	\
		(unsigned char)(mac[4]) &	\
		(unsigned char)(mac[5]) ) == 0xff) ? 1 : 0 \
	
void ip2oct(char* in, unsigned* out);
void int2bstr(unsigned int val, char* bbuf, unsigned int size);
void apply_mask(char *prefix, char mask, char *str_prefix);
void layer2_fill_with_broadcast_mac(char *mac_array);

#define CHKBIT(val) ( (val) ? '1':'0' )
#define SUMIP(arr) (((arr[0])*16777216L) + ((arr[1])*65536L) + ((arr[2]) * 256L) + (arr[3]))
#define OCT2IP(in, out) sprintf(out, "%d.%d.%d.%d", (in)[0],(in)[1],(in)[2],(in)[3])


#endif
