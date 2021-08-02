#include<stdio.h>
#include<stdlib.h>

#ifndef udp_socket
	#include "udp_socket.h"
#endif	

#ifndef msg_io
	#include "msg_io.h"
#endif

#ifndef client_list
	#include "ClientList.h"
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

	// To keep track of known clients
	ClientList *known_clients = make_empty_client_list();
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
	int client_id;
	do{
		// BLOCK till some client sends message
		printf("\n---------------------------------------------------------------");
		printf("\nServer waiting for client messages from all local interfaces...\n\n");
		
		response = wait_for_message(server_sockets, num_sockets, &readable_fds);
		if(response == -9){
			printf("\nTimed out when waiting for messages\nExiting...\n");
			break;
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
					continue;
				}
				else{
					client_id = find_or_add_client(client_addr, known_clients);
					if(client_id==-11){
						printf("\nNew client rejected and acknowledged. Client limit reached!\n");
						msg_size = send_reply(self_socket, SERVER_REJECT_STRING, client_addr, client_addr_len);
						continue;
					}
					else if(strcmp(msg_buffer, TERMINATION_INIT_STRING)==0){
						client_id = remove_client(client_addr, known_clients);
						msg_size = send_reply(self_socket, TERMINATION_ACK_STRING, client_addr, client_addr_len);
						printf("\nClient-%c (%s:%d) Removed from known clients\n", (65+client_id), client_addr_ip_str, client_addr_port);
						continue;
					}
					printf("\nMessage received from \nClient-%c (%s:%d): ", (65+client_id), client_addr_ip_str, client_addr_port);
				}
				printf("%s", msg_buffer);
				// Replying back
				bzero(msg_buffer, MSG_BUFFER_SIZE);
				printf("\nEnter Reply: ");
				scanf(" %[^;]s", msg_buffer);
				// Consume the last newline character from read-buffer
				getchar();   
				msg_size = send_reply(self_socket, msg_buffer, client_addr, client_addr_len);
			}
		}
	}while(1==1);

	destroy_socket(self_socket);
	return;
}