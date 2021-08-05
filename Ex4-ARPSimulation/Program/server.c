#include<stdio.h>
#include<stdlib.h>

#ifndef tcp_socket
	#include "tcp_socket.h"
#endif
#ifndef ARP_Packet_h
	#include "ARP_Packet.h"
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

	// Keep track of all client sockets.
	int num_sockets = 1;
	int *client_sockets = (int*)malloc(sizeof(int)*num_sockets);
	*(client_sockets+0) = client_socket;
	// To keep track of known clients
	ClientList *known_clients = make_empty_client_list();

	struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
	int client_addr_len = sizeof(struct sockaddr_in);
	char *self_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *self_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	char *find_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *find_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	// Store own MAC and IP
	printf("\nEnter Own MAC Address: ");
	scanf(" %s", self_mac);
	printf("Enter Own IP Address: ");
	scanf(" %s", self_ip);	
	
	int msg_size = 0;
	char ch = 'n';
	char *arp_packet_string;
	ARP_Packet *arp_packet;
	do{
		// Accept Destination Address
		printf("Enter Destination IP Address: ");
		scanf(" %s", find_ip);
		// Accept message
		printf("Enter 16-bit Message: ");
		scanf(" %s", msg_buffer);
		// Prepare request packet
		arp_packet = make_arp_packet(REQUEST_OPERATION_ID, self_mac, self_ip, EMPTY_MAC_ADDRESS, find_ip);
		arp_packet_string = serialize_arp_packet(arp_packet);
		printf("\n%s", arp_packet_string);
		// Send request packet
		int msg_size = write(client_socket, arp_packet_string, ARP_PACKET_STRING_SIZE);
		printf("\nARP-Request broadcasted, waiting for response...\n");
		msg_size = receive_message(self_socket, msg_buffer, client_addr, &client_addr_len);
		arp_packet = retrieve_arp_packet(msg_buffer);
		if(arp_packet==NULL){
			printf("\nUnexpected response received...\n");
			printf(" %s\n%s", msg_buffer, client_addr);
		}
		else{

		}
		if (msg_size==0){
			printf("\nClient shut-down abruptly!\n");
			destroy_socket(client_socket);
		}
		printf("\nMore messages to send? (y/n): ");
		scanf(" %c", &ch);
	}while(ch!='n');

	destroy_socket(self_socket);
	return;
}