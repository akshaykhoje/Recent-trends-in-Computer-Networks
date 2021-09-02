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

short find_even_parity(char *rev_merged_msg, int msg_size, int rbit_num){
	// Find's the even parity for posns relevant to rbit_num
	// msg_size is the entire size of merged message
	int start_at = raise_to_power(2, rbit_posn+1);
	// Exclude the r-bit itself
	// Eg: for r2 (rbit_num=1), exclude position-4 (index-3 in the reversed msg)
	count_ones = 0;
	for(i=start_at;i<msg_size;i++){
		// i is the index. posn=i+1
		if(decimal_to_binary(i+1)[rbit_num]==1){
			if(*(rev_merged_msg+i)=='1'){
				count_ones++;
			}
		}
	}
	// Return 1 if odd number of 1s. 0, otherwise
	return count_ones%2;
}

char* position_redundant_bits(char* rev_raw_msg, int msg_size, int r_val){
	// raw_msg is given in reverse
	char *rev_merged_msg = (char*)malloc(sizeof(char)*(msg_size+r_val));
	// curr_r is the r in 2^r=posn (posn in reversed msg)
	for(int i=0, curr_r=0; i<(msg_size+r_val); i++){
		if(raise_to_power(2, curr_r)==(i+1)){  
			// This is a redundant bit
			*(rev_merged_msg+i) = '0';
			curr_r++;
		}
		else{
			*(rev_merged_msg+i) = *(rev_raw_msg+i-curr_r);
		}
	}
	return rev_merged_msg;
}

void make_hamming_message(char* raw_msg){
	int msg_size = strlen(raw_msg);
	int r_val = find_r_value(msg_size);
	char *rev_raw_msg = reverse_string(raw_msg);
	char *rev_merged_msg = position_redundant_bits(rev_raw_msg, msg_size, r_val));
	for(r=0;r<r_val;r++){
		*(rev_merged_msg+raise_to_power(2, r)-1) = 48 + find_even_parity(rev_merged_msg, msg_size+r_val+r);
	}
	printf("%s", rev_merged_msg);
}

#endif
