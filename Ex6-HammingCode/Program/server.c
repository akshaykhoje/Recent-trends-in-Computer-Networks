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

	if (bind_server_socket(self_socket)<0){
		printf("\nCould not bind server socket. Retry!\n");
		destroy_socket(self_socket);
		return;
	}

	if (initiate_listen(self_socket)<0){
		printf("\nCould not listen on server socket. Retry!\n");
		destroy_socket(self_socket);
		return;
	}
	else{
		printf("\nServer listening for connections from all local interfaces...\n");
	}

	struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
	int client_addr_len = sizeof(struct sockaddr_in);
	// BLOCKING routine to accept a client
	int client_socket = accept_client(self_socket, client_addr, &client_addr_len);
	if (client_socket<0){
		printf("\nError when connecting to client. Retry!\n");
		destroy_socket(self_socket);
		return;
	}
	else if(client_addr_len == -1){
		printf("Client connected.\nCould not read address\n");
	}
	else{
		char *client_addr_ip_str = (char*)malloc(sizeof(char)*ADDRESS_BUFFER_SIZE);
		// Alternatively, use inet_ntoa
		inet_ntop(ADDRESS_FAMILY, (void*)&client_addr->sin_addr, client_addr_ip_str, ADDRESS_BUFFER_SIZE);
		int client_addr_port = (int)ntohs(client_addr->sin_port);
		if (client_addr_ip_str == NULL) {
			printf("Client connected.\nCould not read address\n");
		}
		else{
			printf("Connected to Client (%s:%d)\n", client_addr_ip_str, client_addr_port);
		}
	}

	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	char *error_free_msg = NULL;
	int r_value;
	int msg_size = 0;
	do{
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		printf("\nEnter Data: ");
		scanf(" %s", msg_buffer);
		if (check_termination_init(msg_buffer)){
			printf("\nTerminating server...\n");
			destroy_socket(client_socket);
			break;
		}
		msg_buffer = encode_hamming_message(msg_buffer, &r_value, &msg_size, &error_free_msg);
		printf("Number of redundant-bits requied: %d", r_value);
		printf("\nHamming encoded message: %s", error_free_msg);
		printf("\nMessage after adding random noise: %s", msg_buffer);
		msg_size = write(client_socket, msg_buffer, msg_size);
		printf("\n(Data transmitted)\n");
	}while(1==1);

	destroy_socket(self_socket);
	return;
}