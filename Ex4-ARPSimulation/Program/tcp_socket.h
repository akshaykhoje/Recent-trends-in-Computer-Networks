#ifndef tcp_socket 
#define tcp_socket

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#define SERVER_PORT 8080
#define LOCALHOST_IP "127.0.0.1"
#define MAX_CLIENTS 3
#define ADDRESS_FAMILY AF_INET
#define ADDRESS_BUFFER_SIZE 30
#define MSG_BUFFER_SIZE 120
#define TERMINATION_INIT_STRING "ENDSESSION"
#define TERMINATION_ACK_STRING "ENDSESSION_ACK"
#define MSG_DELIMITER ';'
#define IP_ADDRESS_SIZE 20
#define MAC_ADDRESS_SIZE 20
#define MSG_WAIT_TIMEOUT 20

/*	
Use BLOCKING sockets (default configuration)
Only one client-connection
And server only echoes messages
No need to initiate messages on the server!
(i.e) Synchronous send/receive
*/

int make_socket(){
	// AF_INTER specifies IPv4
	// SOCK_STREAM specifies two-way byte-stream
	// 0 selects default protocol
	int sock_fd = socket(ADDRESS_FAMILY, SOCK_STREAM, 0);
	if (sock_fd == -1){
		return -2;    // Could not create socket
	}
	return sock_fd;
}

short check_termination_init(char *msg){
	return (strcmp(msg, TERMINATION_INIT_STRING)==0);
}

short check_termination_ack(char *msg){
	return (strcmp(msg, TERMINATION_ACK_STRING)==0);
}

short bind_server_socket(int sock_fd){
	struct sockaddr_in bind_address;
	// Set family to IPv4
	bind_address.sin_family = ADDRESS_FAMILY;
	// Set port in network byte-order to a non-privileged port (>1023)
	bind_address.sin_port = htons(SERVER_PORT);
	// Set address to 0.0.0.0 to connect to bind to all local interfaces
	bind_address.sin_addr.s_addr = htonl(INADDR_ANY);
	// Convert to generic socket address format and bind
	if (!bind(sock_fd, (struct sockaddr *)&bind_address, sizeof(bind_address))){
		return 0;    // Success
	}
	else{
		printf("%d", errno);
		return -3;   // Could not bind server-socket
	}
}

short connect_server(int sock_fd, char *server_ip){
	struct sockaddr_in bind_address;
	bzero((char*)&bind_address, sizeof(bind_address));
	// Set family to IPv4
	bind_address.sin_family = ADDRESS_FAMILY;
	// Set port in network byte-order to a non-privileged port (>1023)
	bind_address.sin_port = htons(SERVER_PORT);
	// Set address to server IP or 127.0.0.1 to loopback to same host
	if (server_ip == NULL){
		bind_address.sin_addr.s_addr = inet_addr(LOCALHOST_IP);
	}
	else{
		bind_address.sin_addr.s_addr = inet_addr(server_ip);
	}
	// Convert to generic socket address format and bind
	if (!connect(sock_fd, (struct sockaddr*)&bind_address, sizeof(bind_address))){
		return 0;    // Success
	}
	else{
		return -5;   // Could not connect to the local server
	}
}

short initiate_listen(int sock_fd){
	if (!listen(sock_fd, MAX_CLIENTS)){
		return 0;    // Success
	}
	else{
		return -4;   // Could not initiate listening
	}
}

// BLOCKING FUNCTION
int accept_client(int sock_fd, struct sockaddr_in *client_addr, int *client_addr_len){
	// Address of client is registered
	int client_sock_fd = accept(sock_fd, (struct sockaddr*)client_addr, client_addr_len);
	if (client_sock_fd == -1){
		return -5;   // Did not connect to a client
	}
	else if (*client_addr_len > sizeof(struct sockaddr_in)){
		*client_addr_len = -1;    // Non-Fatal Warning: Client address was truncated to fit in buffer
	}
	return client_sock_fd;
}

void destroy_socket(int sock_fd){
	close(sock_fd);
}

#endif