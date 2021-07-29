#ifndef msg_io
#define msg_io 

#include "udp_socket.h"

ssize_t receive_message(int socket, char *msg, struct sockaddr_in *sender_addr, int *sender_addr_len){
	int addr_buffer_size = sizeof(sender_addr)
	int msg_size = recvfrom(socket, msg, MSG_BUFFER_SIZE, 0, (struct sockadd*)sender_addr, sender_addr_len);
	if (*sender_addr_len > addr_buffer_size){
		*sender_addr_len = -1;  // Warning: Client address was truncated to fit in buffer
	}
	return msg_size
}

ssize_t send_message(int socket, char *msg, struct sockaddr_in *destn_addr, int *destn_addr_len){
	// Using MSG_CONFIRM to prevent ARP reprobing, since this is a reply message
	int msg_size = sendto(socket, msg, MSG_BUFFER_SIZE, 0, MSG_CONFIRM, (struct sockaddr*)sender_addr, sender_addr_len);
	if(msg_size == -1){
		return -6;    // Couldn't send message
	}
	return msg_size;
}

ssize_t send_reply(int socket, char *msg, struct sockaddr_in *destn_addr, int *destn_addr_len){
	// Using MSG_CONFIRM to prevent ARP reprobing, since this is a reply message
	int msg_size = sendto(socket, msg, MSG_BUFFER_SIZE, 0, MSG_CONFIRM, (struct sockaddr*)sender_addr, sender_addr_len);
	if(msg_size == -1){
		return -7;    // Couldn't send reply
	}
	return msg_size;
}

#endif msg_io