#ifndef ARP_Packet_h
#define ARP_Packet_h

#include "tcp_socket.h"

#define SEND_OPERATION_ID 1
#define RECEIVE_OPERATION_ID
#define ARP_PACKET_STRING_SIZE 120
#define EMPTY_MAC_ADDRESS "00-00-00-00-00-00"
#define STRING_INIT "-"


struct arp_packet{
	int operation_id;
	char *source_MAC;
	char *source_IP;
	char *destn_MAC;
	char *destn_IP;
};
typedef struct arp_packet ARP_Packet;

ARP_Packet* make_empty_arp_packet(){
	// Allocate memory
	ARP_Packet *packet = (ARP_Packet*)malloc(sizeof(ARP_Packet));
	packet->source_MAC = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	packet->source_IP = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	packet->destn_MAC = (char*)malloc(sizeof(char)*MAC_ADDRESS_SIZE);
	packet->destn_IP = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	packet->operation_id = -1;
	memcpy(packet->source_MAC, STRING_INIT, sizeof(STRING_INIT));
	memcpy(packet->source_IP, STRING_INIT, sizeof(STRING_INIT));
	memcpy(packet->destn_MAC, STRING_INIT, sizeof(STRING_INIT));
	memcpy(packet->destn_IP, STRING_INIT, sizeof(STRING_INIT));
	return packet;
}


ARP_Packet* make_arp_packet(int oper_id, char *source_MAC, char *source_IP, char *destn_MAC, char *destn_IP){
	ARP_Packet* packet = make_empty_arp_packet();
	// Populate the fields
	packet->operation_id = oper_id;
	memcpy(packet->source_MAC, source_MAC, MAC_ADDRESS_SIZE);
	memcpy(packet->source_IP, source_IP, IP_ADDRESS_SIZE);
	memcpy(packet->destn_MAC, destn_MAC, MAC_ADDRESS_SIZE);
	memcpy(packet->destn_IP, destn_IP, IP_ADDRESS_SIZE);
	return packet;
}


ARP_Packet* retrieve_arp_packet(char *packet_str){
	ARP_Packet *packet = make_empty_arp_packet();
	sscanf(packet_str,
		"%d | %s | %s | %s | %s", 
		&packet->operation_id,
		packet->source_MAC,
		packet->source_IP,
		packet->destn_MAC,
		packet->destn_IP
	);
	if(packet->operation_id==-1 ||
		strcmp(packet->source_MAC, STRING_INIT)==0 ||
		strcmp(packet->source_IP, STRING_INIT)==0 ||
		strcmp(packet->destn_MAC, STRING_INIT)==0 || 
		strcmp(packet->destn_IP, STRING_INIT)==0
	){
		return NULL;
	}
	return packet;
}


short is_destn(ARP_Packet *packet, char *self_ip){
	return (strcmp(packet->destn_IP, self_ip)==0);
}


char* serialize_arp_packet(ARP_Packet *packet){
	char *packet_str = (char*)malloc(sizeof(char)*ARP_PACKET_STRING_SIZE);
	sprintf(packet_str,
		"%d | %s | %s | %s | %s", 
		packet->operation_id,
		packet->source_MAC,
		packet->source_IP,
		packet->destn_MAC,
		packet->destn_IP
	);
	return packet_str;
}


void display_arp_packet(ARP_Packet *packet){
	printf("\n%d | %s | %s | %s | %s", 
		packet->operation_id,
		packet->source_MAC,
		packet->source_IP,
		packet->destn_MAC,
		packet->destn_IP
	);
	return;
}

#endif