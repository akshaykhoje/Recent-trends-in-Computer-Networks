#ifndef client_list
#define client_list 

#include "udp_socket.h"

#define MAX_CLIENTS 2

struct linked_list{
  void *data;
  struct linked_list *next;
};

struct clients_list {
	in_addr_t *ips;
	int *ports;
	int count;
};
typedef struct clients_list ClientList;


ClientList* make_empty_client_list(){
	ClientList *handle = (ClientList*)malloc(sizeof(ClientList));
	handle->ips = (in_addr_t*)malloc(sizeof(in_addr_t)*MAX_CLIENTS);
	handle->ports = (int*)malloc(sizeof(int)*MAX_CLIENTS);
	handle->count = 0;
	return handle;
}


int find_or_add_client(struct sockaddr_in *client_addr, ClientList *clients){
	int search_ip = (client_addr->sin_addr).s_addr;
	int search_port = client_addr->sin_port;
	int first_empty = -1;
	int i;
	for(i=0;i<clients->count;i++){
		if( *(clients->ips+i)==search_ip && *(clients->ports+i)==search_port){
			return i;
		}
		else if( *(clients->ips+i)==-1 && *(clients->ports+i)==-1){
			// Found an empty position
			first_empty = i;
		}
	}
	if(clients->count==MAX_CLIENTS){
		// Cannot add new clients now
		return -11;       // Server is busy
	}
	// Add the new client
	if(first_empty!=-1){
		// Use intermediate empty slots if available
		i = first_empty;
	}
	*(clients->ips+i) = search_ip;
	*(clients->ports+i) = search_port;
	clients->count++;
	return i;   // Not found
}


short remove_client(struct sockaddr_in *client_addr, ClientList *clients){
	int search_ip = (client_addr->sin_addr).s_addr;
	int search_port = client_addr->sin_port;
	int first_empty = -1;
	for(int i=0;i<clients->count;i++){
		if( *(clients->ips+i)==search_ip && *(clients->ports+i)==search_port){
			*(clients->ips+i) = -1;
			*(clients->ports+i)= -1;
			clients->count--;
			return i;  // Removed
		}
	}
	return -1;   // Not found
}

#endif 