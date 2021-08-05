#ifndef msg_io
#define msg_io 

#include "tcp_socket.h"

ssize_t receive_message(int socket, char *msg_buffer, struct sockaddr_in *sender_addr, int *sender_addr_len){
	int addr_buffer_size = sizeof(struct sockaddr_in);
	int msg_size = recvfrom(socket, msg_buffer, MSG_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)sender_addr, sender_addr_len);
	if (*sender_addr_len > addr_buffer_size){
		*sender_addr_len = -1;  // Warning: Client address was truncated to fit in buffer
	}
	return msg_size;
}

#endif