#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#ifndef tcp_socket
	#include "tcp_socket.h"
#endif
#ifndef ARP_Packet_h
	#include "ARP_Packet.h"
#endif
#ifndef msg_io
	#include "msg_io.h"
#endif
#ifndef ClientList
	#include "ClientList.h"
#endif

int num_clients = 0;
int self_socket;
int *client_sockets;

void* accept_connections(){
	do{
		struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
		int client_addr_len = sizeof(struct sockaddr_in);
		int client_addr_port;
		char *client_addr_ip_str = (char*)malloc(sizeof(char)*ADDRESS_BUFFER_SIZE);
		int client_socket = accept_client(self_socket, client_addr, &client_addr_len);
		if (client_socket<0){
			continue;
		}
		else{
			client_addr_ip_str = (char*)malloc(sizeof(char)*ADDRESS_BUFFER_SIZE);
			inet_ntop(ADDRESS_FAMILY, (void*)&client_addr->sin_addr, client_addr_ip_str, ADDRESS_BUFFER_SIZE);
			client_addr_port = (int)ntohs(client_addr->sin_port);
			if (client_addr_ip_str == NULL) {
				printf("\n\nNew client connected.\nCould not read address\n");
			}
			else{
				printf("\n\nNew Client Connected (%s:%d)\n", client_addr_ip_str, client_addr_port);
			}
			*(client_sockets+num_clients) = client_socket;
			num_clients++;
		}
	}while(1==1);
}


void main(){
	
	self_socket = make_socket();
	if(self_socket<0){
		printf("\nCould not create socket. Retry!\n");
		return;
	}

	if (bind_server_socket(self_socket)<0){
		printf("\nCould not bind server socket. Retry!\n");
		destroy_socket(self_socket);
		return;
	}

	// Prepare address storage locations
	char *self_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *self_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	// Store own MAC and IP
	printf("\nEnter Own MAC Address: ");
	scanf(" %s", self_mac);
	printf("Enter Own IP Address: ");
	scanf(" %s", self_ip);	

	if (initiate_listen(self_socket)<0){
		printf("\nCould not listen on server socket. Retry!\n");
		destroy_socket(self_socket);
		return;
	}
	else{
		printf("\nServer listening for connections from all local interfaces...\n");
	}

	// Client details storage initialization
	struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
	int client_addr_len = sizeof(struct sockaddr_in);
	int client_addr_port;
	char *client_addr_ip_str = (char*)malloc(sizeof(char)*ADDRESS_BUFFER_SIZE);
	// BLOCKING routine to accept first client
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
		inet_ntop(ADDRESS_FAMILY, (void*)&client_addr->sin_addr, client_addr_ip_str, ADDRESS_BUFFER_SIZE);
		client_addr_port = (int)ntohs(client_addr->sin_port);
		if (client_addr_ip_str == NULL) {
			printf("Client connected.\nCould not read address\n");
		}
		else{
			printf("Connected to Client (%s:%d)\n", client_addr_ip_str, client_addr_port);
		}
	}

	// Keep track of all client sockets.
	client_sockets = (int*)malloc(sizeof(int)*num_clients);
	*(client_sockets+0) = client_socket;
	num_clients++;

	pthread_t listener_thread_id;
  	if(pthread_create(&listener_thread_id, NULL, (void*)(accept_connections), NULL)){
    	printf("\nError while creating thread for AVERAGE\n");
	}

	// Parent process to handle message transmission
	char *find_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *find_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	char *msg_data = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	
	// Prepare data transmission loop
	fd_set readable_fds;
	int msg_size = 0;
	int response;
	int read_fd, write_fd;
	char ch = 'n';
	char *arp_packet_string;
	ARP_Packet *arp_packet;
	do{
		// Accept Destination Address
		printf("\n---------------------------------------------");
		printf("\nEnter Destination IP Address: ");
		scanf(" %s", find_ip);
		// Accept message
		printf("Enter 16-bit Message: ");
		scanf(" %s", msg_data);
		// Prepare request packet
		arp_packet = make_arp_packet(REQUEST_OPERATION_ID, self_mac, self_ip, EMPTY_MAC_ADDRESS, find_ip);
		arp_packet_string = serialize_arp_packet(arp_packet);
		printf("\n%s", arp_packet_string);
		// Send request packet to all clients
		for(int i=0;i<num_clients;i++){
			write_fd = *(client_sockets+i);
			msg_size = write(write_fd, arp_packet_string, ARP_PACKET_STRING_SIZE);
		}
		// BLOCK till some client sends a response
		printf("\nARP-Request broadcasted, waiting for response...\n");		
		response = wait_for_message(client_sockets, num_clients, &readable_fds);
		if(response == -9){
			printf("\nTimed out when waiting for responses\nDropping data...\n");
			continue;
		}
		else if(response == -8){
			printf("\nError occurred when monitoring socket for responses\nRetry!\n");
		}
		// Handle all available readable descriptors
		for(int read_idx=0; read_idx<num_clients; read_idx++){
			read_fd = *(client_sockets+read_idx);		
			if (FD_ISSET(read_fd, &readable_fds)==0){
				// This socket is not readable
				continue;
			}			
			msg_size = receive_message(read_fd, msg_buffer, client_addr, &client_addr_len);
			arp_packet = retrieve_arp_packet(msg_buffer);
			if(arp_packet==NULL){
				printf("\nUnexpected response received...\n");
				printf("%s\n%s", msg_buffer);
			}
			else{
				printf("\nARP-Response Received\n%s", msg_buffer);
				msg_size = write(read_fd, msg_data, MSG_BUFFER_SIZE);
				printf("\n\nData transmitted");
			}
		}
		printf("\nMore messages to send? (y/n): ");
		scanf(" %c", &ch);
	}while(ch!='n');

	destroy_socket(self_socket);
	return;
}