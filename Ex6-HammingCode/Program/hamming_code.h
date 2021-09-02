#ifndef hamming_code_h
#define hamming_code_h

#include<stdlib.h>
#include<string.h>
#include<math.h>


char* decimal_to_binary(int num){
	// Result is returned in reverse order
	// Eg: 6 -> 011
	int binary_size = (int)ceil(log2(num)) + 1;
	char* binary = (char*)malloc(sizeof(char)*binary_size);
	for(int i=0;i<binary_size;i++){
		// Converting to ASCII character
		*(binary+i) = 48 + num%2; 
		num /= 2;
	}
	return binary;
}


int raise_to_power(int base, int exp){
	// Only for positive exp
	int result = 1;
	for(int i=0;i<exp;i++){
		result *= base;
	}
	return result;
}

char* reverse_string(char *string){
	int size = strlen(string);
	char *rev_string = (char*)malloc(sizeof(char)*size);
	for(int i=0;i<size;i++){
		*(rev_string+i) = *(string+size-i-1);
	}
	return rev_string;
}

int find_r_value(int msg_size){
	// Deduce r from 2^r >= m+r+1
	for(int r=0;r<msg_size;r++){
		if(raise_to_power(2, r) >= msg_size+r+1){
			return r;
		}
	}
	return -1;  // Not found. Syntactic statement - never reached
}

char* position_redundant_bits(char* rev_raw_msg, int msg_size, int r_val){
	// Redundant bits are set to 0 initially
	// raw_msg is given in reverse
	char *merged_msg = (char*)malloc(sizeof(char)*(msg_size+r_val));
	for(int i=0, curr_r=0; i<(msg_size+r_val); i++){
		if(raise_to_power(2, curr_r)==(i+1)){  
			// This is a redundant bit
			*(merged_msg+i) = '0';
			curr_r++;
		}
		else{
			*(merged_msg+i) = *(merged_msg+i-curr_r);
		}
	}
	return merged_msg;
}

void make_hamming_message(char* raw_msg){
	int msg_size = strlen(raw_msg);
	int r_val = find_r_value(msg_size);
	char *rev_raw_msg = reverse_string(raw_msg);
	printf(" %s", position_redundant_bits(rev_raw_msg, msg_size, r_val));
}

#endif
