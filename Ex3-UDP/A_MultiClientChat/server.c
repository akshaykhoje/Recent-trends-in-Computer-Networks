#include<stdio.h>
#include<stdlib.h>

#ifndef udp_socket
	#include "udp_socket.h"
#endif	

#ifndef msg_io
	#include "msg_io.h"
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

	// Keep track of all server sockets.
	int num_sockets = 1;
	int *server_sockets = (int*)malloc(sizeof(int)*num_sockets);
	*(server_sockets+0) = self_socket;

	// To store sender-client address
	struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
	int client_addr_len = sizeof(struct sockaddr_in);
	char *client_addr_ip_str = (char*)malloc(sizeof(char)*ADDRESS_BUFFER_SIZE);
	int client_addr_port;

	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	fd_set readable_fds;
	int msg_size = 0;
	int response;
	int read_fd;
	do{
		// BLOCK till some client sends message
		printf("\nServer waiting for client messages from all local interfaces...\n");
		fflush(stdout);
		
		response = wait_for_message(server_sockets, num_sockets, &readable_fds);
		printf("%d", response);
		if(response == -9){
			printf("\nTimed out when waiting for messages\nExiting...\n");
			return;
		}
		else if(response == -8){
			printf("\nError occurred when monitoring socket for messages\nRetry!\n");
		}

		// Handle all available readable descriptors
		for(int read_idx=0; read_idx<num_sockets; read_idx++){
			read_fd = *(server_sockets+read_idx);
		
			if (FD_ISSET(read_fd, &readable_fds)==0){
				// This socket is not readable
				continue;
			}
			
			msg_size = receive_message(self_socket, msg_buffer, client_addr, &client_addr_len);
			printf("%s", msg_buffer);
			if (msg_size==0){
				printf("\nEmpty message\n");
			}
			else if(client_addr_len == -1){
				printf("\nMessage received from Client.\nCould not read address\n");
			}
			else{
				// Alternatively, use inet_ntoa
				inet_ntop(ADDRESS_FAMILY, (void*)&client_addr->sin_addr, client_addr_ip_str, ADDRESS_BUFFER_SIZE);
				client_addr_port = (int)ntohs(client_addr->sin_port);
				if (client_addr_ip_str == NULL) {
					printf("\nMessage received from Client.\nCould not read address\n");
				}
				else{
					printf("\nMessage received from Client (%s:%d)\n", client_addr_ip_str, client_addr_port);
				}
				// Echoing back
				printf("\n%s", msg_buffer);
				msg_size = send_reply(self_socket, msg_buffer, client_addr, client_addr_len);
				printf("\n(Message echoed back)\n");
			}
		}
	}while(1==1);

	destroy_socket(self_socket);
	return;
}