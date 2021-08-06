#include<stdio.h>
#include<stdlib.h>

#ifndef udp_socket
	#include "udp_socket.h"
#endif	

#ifndef msg_io
	#include "msg_io.h"
#endif	

#ifndef DNS_Table_h
	#include "DNSTable.h"
#endif

// Global DNS Table
DNS_Table* dns_table = NULL;

void* update_dns_table(){
	char response;
	char *domain_name = (char*)malloc(sizeof(char)*DOMAIN_NAME_SIZE);
	char *ip_address = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	do{
		scanf(" %c", response);
		if(response=='u'||response=='U'){
			printf("\nEnter Domain Name: ");
			scanf(" %s", domain_name);
			printf("\nEnter IP Address: ");
			scanf(" %s", ip_address);
			dns_table = add_dns_ip(domain_name, ip_address, dns_table);
			display_dns_table(dns_table);
		}
	}while(1==1);
}


void main(){
	
	// Populate DNS table
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
	// To store sender-client address
	struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
	int client_addr_len = sizeof(struct sockaddr_in);
	char *client_addr_ip_str = (char*)malloc(sizeof(char)*ADDRESS_BUFFER_SIZE);
	int client_addr_port;
	// Data transmission storage locations
	DNS_Table *dns_entry;
	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	fd_set readable_fds;
	int msg_size = 0;
	int response;
	int read_fd;
	do{
		// BLOCK till some client sends message
		printf("\n---------------------------------------------------------------");
		printf("\nWaiting for DNS requests... Hit 'u' to update DNS table\n");
		
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
				printf("\nRequest received\nCould not read sender address!\n");
			}
			else{
				// Alternatively, use inet_ntoa
				inet_ntop(ADDRESS_FAMILY, (void*)&client_addr->sin_addr, client_addr_ip_str, ADDRESS_BUFFER_SIZE);
				client_addr_port = (int)ntohs(client_addr->sin_port);
				if (client_addr_ip_str == NULL) {
					printf("\nRequest received\nCould not read sender address!\n");
				}
				else{
					printf("\nRequest received from %s:%d", client_addr_ip_str, client_addr_port);
				}
				
				printf("\n\nDomain Requested: %s", msg_buffer);
				dns_entry = get_dns_entry(msg_buffer, dns_table);
				if(dns_entry==NULL){
					printf("\nNo IP-Address found");
					memcpy(msg_buffer, "No IP-Address found", MSG_BUFFER_SIZE);
					msg_size = send_reply(self_socket, msg_buffer, client_addr, client_addr_len);
				}
				else{
					printf("\nIP-Addresses Responded");
					for(int i=0;i<dns_entry->num_ips;i++){
						memcpy(msg_buffer, *(dns_entry->ips+i), MSG_BUFFER_SIZE);
						msg_size = send_reply(self_socket, msg_buffer, client_addr, client_addr_len);
					}
				}
				msg_size = send_reply(self_socket, RESPONSE_END_STRING, client_addr, client_addr_len);
			}
		}
	}while(1==1);

	destroy_socket(self_socket);
	return;
	
}