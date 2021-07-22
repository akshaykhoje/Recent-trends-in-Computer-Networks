#include<sys/socket.h>

#define SERVER_PORT 8080
#define CLIENT_PORT 8000
#define BACKLOG_LIMIT 5
#define LOCALHOST_IP "127.0.0.1"

/*	
Use BLOCKING sockets (default configuration)
Only one client-connection
And server only echoes messages
No need to initiate messages on the server!
*/

int make_socket(){
	// AF_INTER specifies IPv4
	// SOCK_STREAM specifies two-way byte-stream
	// 0 selects default protocol
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1){
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

// BLOCKING FUNCTION
int connect_server(int sock_fd){
	struct sockaddr_in bind_address;
	// Set family to IPv4
	bind_address.sin_family = AF_INET;
	// Set port in network byte-order to a non-privileged port (>1023)
	bind_address.sin_port = htons(CLIENT_PORT);
	// Set address to 127.0.0.1 to loopback to same host
	bind_address.sin_addr.s_addr = inet_addr(LOCALHOST_IP);
	// Convert to generic socket address format and bind
	if (!connect(sock_fd, (struct sockaddr *)&bind_address, sizeof(bind_address))){
		return 1;    // Success
	}
	else{
		return -5;   // Could not connect to the local server
	}
}

int initiate_listening(int sock_fd){
	if (!listen(sock_fd, BACKLOG_LIMIT)){
		return 1;    // Success
	}
	else{
		return -4;   // Could not initiate listening
	}
}

// BLOCKING FUNCTION
int accept_client(int sock_fd){
	// Address of client is not required
	client_sock_fd = accept(sock_fd, NULL, NULL);
	if (client_sock_fd == -1){
		return -5;   // Did not connect to a client
	}
	return cient_sock_fd;
}