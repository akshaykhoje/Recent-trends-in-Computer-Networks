#include<stdio.h>
#include<stdlib.h>

#ifndef tcp_socket
	#include "tcp_socket.h"
#endif
#ifndef ARP_Packet_h
	#include "ARP_Packet.h"
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

	char *self_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *self_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	char *find_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *find_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);

	printf("\nEnter Own MAC Address: ");
	scanf(" %s", self_mac);
	printf("Enter Own IP Address: ");
	scanf(" %s", self_ip);

	printf("Enter Destination IP Address: ");
	scanf(" %s", find_ip);

	printf("Enter 16-bit Message: ");
	scanf(" %s", msg_buffer);

	ARP_Packet *arp_packet = make_arp_packet(SEND_OPERATION_ID, self_mac, self_ip, EMPTY_MAC_ADDRESS, find_ip);
	char *arp_packet_string = serialize_arp_packet(arp_packet);
	printf("\n%s", arp_packet_string);

	int msg_size = write(client_socket, arp_packet_string, ARP_PACKET_STRING_SIZE);
	printf("\n(ARP-Packet Broadcasted)\n");
	msg_size = read(client_socket, msg_buffer, MSG_BUFFER_SIZE);
	printf(" %s", msg_buffer);
	if (msg_size==0){
		printf("\nClient shut-down abruptly!\n");
		destroy_socket(client_socket);
	}
	if (check_termination_init(msg_buffer)){
		printf("\nClient terminated connection\n");
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		msg_size = write(client_socket, TERMINATION_ACK_STRING, msg_size);
		destroy_socket(client_socket);
	}
	printf("\nCLIENT pinged: %s", msg_buffer);
	msg_size = write(client_socket, msg_buffer, msg_size);
	printf("\n(Message echoed back)\n");

	/*
	int msg_size = 0;
	do{
		// BLOCKING routine to wait for a message
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		msg_size = read(client_socket, msg_buffer, MSG_BUFFER_SIZE);
		if (msg_size==0){
			printf("\nClient shut-down abruptly!\n");
			destroy_socket(client_socket);
			break;
		}
		// Echo back
		if (check_termination_init(msg_buffer)){
			printf("\nClient terminated connection\n");
			bzero(msg_buffer, MSG_BUFFER_SIZE);
			msg_size = write(client_socket, TERMINATION_ACK_STRING, msg_size);
			destroy_socket(client_socket);
			break;
		}
		printf("\nCLIENT pinged: %s", msg_buffer);
		msg_size = write(client_socket, msg_buffer, msg_size);
		printf("\n(Message echoed back)\n");
	}while(1==1);
	*/

	destroy_socket(self_socket);
	return;
}