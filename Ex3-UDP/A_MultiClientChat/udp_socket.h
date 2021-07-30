#ifndef udp_socket
#define udp_socket

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#define SERVER_PORT 8088
#define LOCALHOST_IP "127.0.0.1"
#define ADDRESS_FAMILY AF_INET
#define ADDRESS_BUFFER_SIZE 30
#define MSG_BUFFER_SIZE 100
#define IP_STRING_LEN 24	
#define TERMINATION_INIT_STRING "ENDSESSION"
#define TERMINATION_ACK_STRING "ENDSESSION_ACK"
#define SERVER_REJECT_STRING "SERVER_IS_BUSY"
#define MSG_DELIMITER ';'
#define MSG_WAIT_TIMEOUT 20

/*	
Use BLOCKING sockets (default configuration)
Alternating send-receive only
No need to initiate messages on the server!
(i.e) Synchronous send/receive
*/

int make_socket(){
	// AF_INTER specifies IPv4
	// SOCK_STREAM specifies two-way byte-stream
	// 0 selects default protocol
	int sock_fd = socket(ADDRESS_FAMILY, SOCK_DGRAM, IPPROTO_UDP);
	if (sock_fd == -1){
		return -2;    // Could not create socket
	}
	return sock_fd;
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

struct sockaddr_in wrap_address(char *ip_address, int port){
	struct sockaddr_in address;
	bzero((char*)&address, sizeof(address));
	// Set family to IPv4
	address.sin_family = ADDRESS_FAMILY;
	// Set port in network byte-order to a non-privileged port (>1023)
	address.sin_port = htons(port);
	// Set the ip address in byte format
	address.sin_addr.s_addr = inet_addr(ip_address);
	return address;
}

void destroy_socket(int sock_fd){
	close(sock_fd);
}

#endif