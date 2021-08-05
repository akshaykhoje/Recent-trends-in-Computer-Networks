#ifndef client_list
#define client_list 

#include "tcp_socket.h"

struct clients_list {
	int *conn_fds;
	in_addr_t *ips;
	int *ports;
	int count;
};
typedef struct clients_list ClientList;


ClientList* make_empty_client_list(){
	ClientList *handle = (ClientList*)malloc(sizeof(ClientList));
	handle->conn_fds = (int*)malloc(sizeof(int));
	handle->ips = (in_addr_t*)malloc(sizeof(in_addr_t));
	handle->ports = (int*)malloc(sizeof(int));
	handle->count = 0;
	return handle;
}

void add_client(int conn_fd, struct sockaddr_in *client_addr, ClientList *clients){
	int client_count = clients->count;
	clients->conn_fds = (int*)realloc(clients->conn_fds, sizeof(int)*client_count);
	clients->ips = (in_addr_t*)realloc(clients->ips, sizeof(in_addr_t)*client_count);
	clients->ports = (int*)realloc(clients->ports, sizeof(int)*client_count);
	*(clients->conn_fds+clients->count) = conn_fd;
	*(clients->ips+clients->count) = (client_addr->sin_addr).s_addr;
	*(clients->ports+clients->count) = client_addr->sin_port;
	clients->count++;
}


int find_or_add_client(int search_fd, ClientList *clients){
	for(int i=0;i<clients->count;i++){
		if( *(clients->conn_fds+i)==search_fd){
			return i;
		}
	}
	return -1; //Not found
}

#endif 