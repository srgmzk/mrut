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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define STR32ZERO "00000000000000000000000000000000\0"

void
dec2binstr(unsigned val, char *buf) {
	unsigned i = 31;
	unsigned val_ = val;
	unsigned tmp;
	while ( i ) {

		tmp = val_ % 2;
	
		if ( tmp ) buf[i] = '1';
		printf("val_ = %d, tmp = %d, i=%d, \nbuf = %s\n", val_, tmp, i, buf);
		--i;
		val_ /= 2;
	}
    		
	buf[i] = (val_) ? '1':'0' ;
	printf("%s %d %d\n", buf, i, val_);
						
}

int main() {

	char *in = "255.8.0.255";
	unsigned buf[4] = {0,0,0,0};
	unsigned long out;

	/* (first octet * 16777216) + (second octet * 65536) + (third octet * 256) + (fourth octet) */
	out = sscanf(in, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]); 
	out = buf[0]*16777216L + buf[1]*65536L + buf[2]*256L + buf[3];
	
	//printf("1). %lu\n", out);


	int i = 0;
	char bins[33] = STR32ZERO;
	char oct[8] = "00000000";
	printf("%ld \n", out);
	dec2binstr(out, bins);
	//printf("2). %s\n",bins);



	while (i < 26) { 
		memcpy(oct, &bins[i], 8);
		oct[8] = '\0';
		printf("3). %d\n", (unsigned int)strtol(oct,NULL,2));
		i+=8;		
	}
	
	return out;
}
