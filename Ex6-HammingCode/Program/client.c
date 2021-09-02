#include<stdio.h>
#include<stdlib.h>

#include "tcp_socket.h"

#ifndef hamming_code_h
	#include "hamming_code.h"
#endif

void main(){

	int self_socket = make_socket();
	if(self_socket<0){
		printf("\nCould not create socket. Retry!\n");
		return;
	}

	char *server_ip = (char*)malloc(sizeof(char)*IP_STRING_LEN);
	printf("\nEnter Server IP Address: ");
	scanf(" %s", server_ip);
	if (connect_server(self_socket, server_ip) < 0){
		printf("\nCould not connect to Server.\nMake sure the server is running!\n");
		destroy_socket(self_socket);
		return;
	}
	else{
		printf("\nConnected to Server");
	}

	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	char *redundant_bits = NULL;
	int msg_size = 0;
	do {
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		msg_size = read(self_socket, msg_buffer, MSG_BUFFER_SIZE);
		if (msg_size==0){
			printf("\nServer exited...\n");
			break;
		}
		printf("Data received: %s\n", msg_buffer);
		msg_buffer = decode_hamming_message(msg_buffer, &redundant_bits);
		printf("Redundant bits computed: %s", redundant_bits);
		printf("\nCorrected data: %s", msg_buffer);
		fflush(stdout);
	}while(1==1);
}