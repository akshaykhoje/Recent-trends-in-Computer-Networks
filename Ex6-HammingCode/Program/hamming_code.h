#ifndef hamming_code_h
#define hamming_code_h

#include<stdlib.h>

int raise_to_power(int base, int exp){
	// Only for positive exp
	int result = 1;
	for(int i=0;i<exp;i++){
		result *= base;
	}
	return result;
}

int find_r_value(int msg_size){
	// Deduce r from 2^r >= m+r+1
	for(int r=0;i<m;i++){
		if raise_to_power(2, r) >= m+r+1{
			return r
		}
	}
	return -1;  // Not found. Syntactic statement - never reached
}

#endif
