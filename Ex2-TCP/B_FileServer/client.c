#include<stdio.h>
#include<stdlib.h>

#ifndef tcp_socket
	#include "tcp_socket.h"
#endif	

#ifndef file_io
	#include "file_io.h"
#endif

void main(){

	int self_socket = make_socket();
	if(self_socket<0){
		printf("\nCould not create socket. Retry!\n");
		return;
	}

	if (connect_server(self_socket) < 0){
		printf("\nCould not connect to ECHO-Server.\nMake sure the server is running!\n");
		destroy_socket(self_socket);
		return;
	}
	else{
		printf("\nConnected to ECHO-Server");
	}

	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	int msg_size = 0;
	int response;
	printf("\n\nEnter 'ENDSESSION' to terminate connection\n");
	do {
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		printf("\nEnter File Name: ");
		scanf(" %[^\n]s", msg_buffer);   
		msg_size = write(self_socket, msg_buffer, MSG_BUFFER_SIZE);
		if (check_termination_init(msg_buffer)){
			msg_size = read(self_socket, msg_buffer, MSG_BUFFER_SIZE);
			printf("\n%s", msg_buffer);
			// Check if server acknowledged ENDSESSION
			if (check_termination_ack(msg_buffer)){
				printf("\nExiting...\n");
				break;
			}
			else{
				printf("\nServer did not acknowledge termination. Retry!\n");
				continue;
			}
		}
		response = receive_file(msg_buffer, self_socket);
		if(response==-8){
			printf("\nCould not create local file\n");
		}
	}while(1==1);
}