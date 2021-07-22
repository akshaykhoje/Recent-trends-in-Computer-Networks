#include<sys/socket.h>

#define SERVER_PORT 8080

int make_socket(){
	// AF_INTER specifies IPv4
	// SOCK_STREAM specifies two-way byte-stream
	// 0 selects default protocol
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if sock_fd == -1{
		return -2;    // Could not create socket
	}
	return sock_fd;
}

int bind_server_socket(int sock_fd){
	struct sockaddr_in bind_address;
	// Set family to IPv4
	bind_address.sin_family = AF_INET;
	// Set port in network byte-order to a non-privileged port (>1023)
	bind_address.sin_port = htons(SERVER_PORT);
	// Set address to 0.0.0.0 to connect to bind to all local interfaces
	bind_address.sin_addr.s_addr = htonl(INADDR_ANY);
	// Convert to generic socket address format and bind
	if (!bind(sock_fd, (struct sockaddr *)&bind_address, sizeof(bind_address))){
		return 1;    // Success
	}
	else{
		return -3;   // Could not bind server-socket
	}
}