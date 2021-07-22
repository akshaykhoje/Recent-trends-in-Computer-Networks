#include<stdio.h>
#include<stdlib.h>

#include "network.h"

void main(){

	int self_socket = make_socket();
	if(self_socket<0){
		printf("\nCould not create socket. Retry!\n");
		return;
	}

	if (connect_server(self_socket) < 0){
		printf("\nCould not conect to a server. Retry!\n");
		destroy_socket(self_socket);
		return;
	}
}