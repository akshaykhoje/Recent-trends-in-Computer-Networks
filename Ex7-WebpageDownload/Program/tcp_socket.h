#ifndef tcp_socket_h
#define tcp_socket_h

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#define SERVER_PORT 8080
#define BACKLOG_LIMIT 5
#define LOCALHOST_IP "127.0.0.1"
#define ADDRESS_FAMILY AF_INET
#define ADDRESS_BUFFER_SIZE 30
#define BUFFER_SIZE 1000
#define IP_STRING_LEN 24	
#define MSG_DELIMITER ';'
#define HTTP_PORT "80"

typedef struct addrinfo AddrInfo;

/*	
Use BLOCKING sockets (default configuration)
Only one client-connection
And server only echoes messages
No need to initiate messages on the server!
(i.e) Synchronous send/receive
*/

int make_socket(AddrInfo *config){
	// AF_INTER specifies IPv4
	// SOCK_STREAM specifies two-way byte-stream
	// 0 selects default protocol
	int sock_fd = socket(config->ai_family, config->ai_socktype, config->ai_protocol);
	if (sock_fd == -1){
		return -2;    // Could not create socket
	}
	return sock_fd;
}

short connect_server(int sock_fd, AddrInfo *config){
	// Convert to generic socket address format and bind
	if (!connect(sock_fd, config->ai_addr, config->ai_addrlen)){
		return 0;    // Success
	}
	else{
		return -3;   // Could not connect to the local server
	}
}

void destroy_socket(int sock_fd){
	close(sock_fd);
}

#endif
