#ifndef ARP_Packet
#define ARP_Packet

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
	return packet;
}


ARP_Packet* make_arp_packet(int oper_id, char *source_MAC, char *source_IP, char *destn_MAC, char *destn_IP){
	ARP_Packet* packet = make_empty_arp_packet();
	// Populate the fields
	packet->operation_id = oper_id;
	memcpy(packet->source_MAC, source_MAC, MAC_ADDRESS_SIZE);
	memcpy(packet->source_IP, source_IP, IP_ADDRESS_SIZE);
	memcpy(packet->destn_MAC, destn_MAC, MAC_ADDRESS_SIZE);
	memcpy(packet->desnt_IP, destn_IP, IP_ADDRESS_SIZE);
	return packet;
}


ARP_Packet* retreive_arp_packet(char *packet_str){
	ARP_Packet *packet = make_empty_arp_packet();
	sscanf("%d | %s | %s | %s | %s", 
		packet->operation_id,
		packet->source_MAC,
		packet->source_IP,
		packet->destn_MAC,
		packet->destn_IP
	)
	return packet;
}


void display_arp_packet(ARP_Packet *packet){
	printf("%d | %s | %s | %s | %s", 
		packet->operation_id,
		packet->source_MAC,
		packet->source_IP,
		packet->destn_MAC,
		packet->destn_IP
	)
	return;
}

#endif