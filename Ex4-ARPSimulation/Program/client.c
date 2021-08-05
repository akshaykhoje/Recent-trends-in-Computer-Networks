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

	char *server_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	printf("\nEnter Host-IP Address: ");
	scanf(" %s", server_ip);
	if (connect_server(self_socket, server_ip) < 0){
		printf("\nCould not connect to Host.\nMake sure the server is running!\n");
		destroy_socket(self_socket);
		return;
	}
	else{
		printf("\nConnected to Host");
	}

	char *self_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *self_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	char *find_mac = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	char *find_ip = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	// Store own MAC and IP
	printf("\n\nEnter Own MAC Address: ");
	scanf(" %s", self_mac);
	printf("Enter Own IP Address: ");
	scanf(" %s", self_ip);
	// Allocate message memory
	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	char *arp_packet_string;
	int msg_size = 0;

	do {
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		msg_size = read(self_socket, msg_buffer, MSG_BUFFER_SIZE);
		if(msg_size==0){
			printf("\nHost has terminated\nExiting...\n");
			break;
		}
		// Attempt packet read, else print message
		ARP_Packet *arp_packet = retrieve_arp_packet(msg_buffer);
		if(arp_packet==NULL){
			printf("\nMessage from host: %s\n", msg_buffer);
		}
		else{
			printf("ARP Request Recieved\n%s", msg_buffer);
			if(is_destn(arp_packet, self_ip)){
				printf("IP Address Matched\n");
				arp_packet = make_arp_packet(RESPONSE_OPERATION_ID, self_mac, self_ip, arp_packet->source_MAC, arp_packet->source_IP);
				arp_packet_string = serialize_arp_packet(arp_packet);			
				msg_size = write(self_socket, arp_packet_string, ARP_PACKET_STRING_SIZE);
				printf("\n%s", arp_packet_string);	
				printf("\n(ARP Response Sent)\n");
			}
			else{
				printf("IP Address Did NOT Match\n");
			}
		}
		fflush(stdout);
	}while(1==1);
}