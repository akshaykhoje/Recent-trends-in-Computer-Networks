#ifndef msg_io
#define msg_io 

#include<

#include "udp_socket.h"

struct timeval prepare_time_structure(int duration_sec, int duration_usec){
	struct timeval time;
	time.tv_sec = duration_sec;
	time.tv_usec = duration_usec;
	return time;
}

int wait_for_message(int *server_fds, int num_fds, fd_set *avl_fds){
	fd_set read_fds;
	FD_ZERO(&fd_set);
	for(int i=0;i<num_fds;i++){
		FD_SET(*(server_fds+0), &fd_set);
	}
	int avl_fds_count = select(num_fds, &read_fds, NULL, NULL, &prepare_time_structure(MSG_WAIT_TIMEOUT, 0));
	if(avl_fds_count==-1){
		return -8;    // Error when waiting for socket
	}
	else ifa(avl_fds_count==0){
		return -9;    // Timed-out when waiting for message
	}
	return avl_fds_count;
}

ssize_t receive_message(int socket, char *msg, struct sockaddr_in *sender_addr, int *sender_addr_len){
	int addr_buffer_size = sizeof(sender_addr)
	int msg_size = recvfrom(socket, msg, MSG_BUFFER_SIZE, 0, (struct sockadd*)sender_addr, sender_addr_len);
	if (*sender_addr_len > addr_buffer_size){
		*sender_addr_len = -1;  // Warning: Client address was truncated to fit in buffer
	}
	return msg_size
}

ssize_t send_message(int socket, char *msg, struct sockaddr_in *destn_addr, int destn_addr_len){
	// Using MSG_CONFIRM to prevent ARP reprobing, since this is a reply message
	int msg_size = sendto(socket, msg, MSG_BUFFER_SIZE, 0, MSG_CONFIRM, (struct sockaddr*)destn_addr, destn_addr_len);
	if(msg_size == -1){
		return -6;    // Couldn't send message
	}
	return msg_size;
}

ssize_t send_reply(int socket, char *msg, struct sockaddr_in *destn_addr, int destn_addr_len){
	// Using MSG_CONFIRM to prevent ARP reprobing, since this is a reply message
	int msg_size = sendto(socket, msg, MSG_BUFFER_SIZE, 0, MSG_CONFIRM, (struct sockaddr*)destn_addr, destn_addr_len);
	if(msg_size == -1){
		return -7;    // Couldn't send reply
	}
	return msg_size;
}

#endif msg_io