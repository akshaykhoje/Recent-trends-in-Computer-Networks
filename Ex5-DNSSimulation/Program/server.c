#include<stdio.h>
#include<stdlib.h>

#ifndef udp_socket
	#include "udp_socket.h"
#endif	

#ifndef DNS_Table_h
	#include "DNSTable.h"
#endif


void main(){

	DNS_Table* dns_table = NULL;
	dns_table = add_dns_ip("www.google.com", "192.168.0.1", dns_table);
	dns_table = add_dns_ip("www.google.com", "192.167.0.1", dns_table);
	dns_table = add_dns_ip("www.yahoo.com", "192.67.0.1", dns_table);
	display_dns_table(dns_table);
	
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

	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	fd_set readable_fds;
	int msg_size = 0;
	int response;
	int read_fd;
	do{
		// BLOCK till some client sends message
		printf("\n---------------------------------------------------------------");
		printf("\nServer waiting for DNS request from all local interfaces...\n\n");
		
		response = wait_for_message(server_sockets, num_sockets, &readable_fds);
		if(response == -9){
			printf("\nTimed out when waiting for requests\nExiting...\n");
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
					printf("\nMessage received from \nClient (%s:%d): ", client_addr_ip_str, client_addr_port);
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
	*/
}