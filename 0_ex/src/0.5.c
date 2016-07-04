/*
 * 0.5.c
 *
 *  Created on: Apr 7, 2016
 *      Author: Jiadong
 */

#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>


void rev_bit(uint8_t input)
{
	uint8_t output = 0;
	uint8_t temp = input;
	int i=7;
	while(i >= 0){
		temp = input & 1; //every time take one bit from the input
		input = input>>1; //
		printf("input %x, temp %x\n",input, temp);
		output = output |( temp<< i);
		printf("output %x\n",output);
		i--;
	}
	printf("the output is %u",output);
}

int main( ){

	rev_bit(240);

	return 1;
}
