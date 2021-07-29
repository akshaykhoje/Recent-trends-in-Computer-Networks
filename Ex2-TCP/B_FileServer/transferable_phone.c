#ifndef tcp_socket
#define tcp_socket

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

#define SERVER_PORT 8080
#define BACKLOG_LIMIT 5
#define LOCALHOST_IP "127.0.0.1"
#define ADDRESS_FAMILY AF_INET
#define ADDRESS_BUFFER_SIZE 30
#define MSG_BUFFER_SIZE 20
#define IP_STRING_LEN 24
#define TERMINATION_INIT_STRING "ENDSESSION"
#define TERMINATION_ACK_STRING "ENDSESSION_ACK"
#define FILENOTFOUND_STRING "FILENOTFOUND"
#define TRANSFERFAIL_STRING "TRANFERFAILED"
#define MSG_DELIMITER ';'

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
	if (!listen(sock_fd, BACKLOG_LIMIT)){
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

#ifndef file_io
#define file_io

#include<fcntl.h>
#include<sys/stat.h>

#define FILE_CHUNK_ACK "FILECHUNKACK"
#define EOF_STRING "ENDFILEHERE"
#define PATH_SIZE 100

mode_t get_default_fileperm(){
    mode_t curr_umask = umask(022);
    umask(curr_umask);
    return (mode_t)0666-curr_umask;
}

short check_chunk_ack(char *msg){
	return (strcmp(msg, FILE_CHUNK_ACK)==0);
}

short check_eof(char *msg){
	return (strcmp(msg, EOF_STRING)==0);
}

short check_transfer_fail(char *msg){
	return (strcmp(msg, TRANSFERFAIL_STRING)==0);
}

short check_filenotfound(char *msg){
	return (strcmp(msg, FILENOTFOUND_STRING)==0);
}

short send_file(char *filepath, int sock_fd){
	int fd_in = open(filepath, O_RDONLY);
	if (fd_in<0){
		return -6;   // Could not open read-file
	}
	char *data = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	int read_size = read(fd_in, data, MSG_BUFFER_SIZE);
	int write_size = 0;
	do{
		write_size = write(sock_fd, data, read_size);
		// Wait for acknowledgement
		read_size = read(sock_fd, data, MSG_BUFFER_SIZE);
		if (!check_chunk_ack(data)){
			return -7;    // Corrupt occurred while sending file
		}
		read_size = read(fd_in, data, MSG_BUFFER_SIZE);		
	}while(read_size!=0);
	write_size = write(sock_fd, EOF_STRING, sizeof(EOF_STRING));
	close(fd_in);
	return 0;
}


short receive_file(char *filepath, int sock_fd){
	char *working_dir = (char*)malloc(sizeof(char)*PATH_SIZE);
	getcwd(working_dir, PATH_SIZE);
	chdir("/storage/3426-1912/Movies");
	int fd_out = open(filepath, O_CREAT|O_WRONLY, get_default_fileperm());
	chdir(working_dir);
	if(fd_out<0){
		return -8;   // Could not open write-file
	}
	char *buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	// Block until filename is read
	int read_size = 0;
	do{
		read_size = recv(sock_fd, buffer, MSG_BUFFER_SIZE, MSG_PEEK);			
	}while(strcmp(buffer, filepath)==0);
	// Start file-transfer
	read_size = read(sock_fd, buffer, MSG_BUFFER_SIZE);
	if (check_transfer_fail(buffer)){
		return -9;   // Transfer failed abruptly
	}
	else if(check_filenotfound(buffer)){
		return -10;  // File was not found
	}
	int write_size = 0;
	do{
		if (check_eof(buffer)) {
			break;
		}
		write_size = write(fd_out, buffer, read_size);
		// Send acknowledgement
		write_size = write(sock_fd, FILE_CHUNK_ACK, sizeof(FILE_CHUNK_ACK));
		// Block until acknowledgement is read
		do{
			read_size = recv(sock_fd, buffer, MSG_BUFFER_SIZE, MSG_PEEK);			
		}while(check_chunk_ack(buffer));
		// Read next chunk of data
		read_size = read(sock_fd, buffer, MSG_BUFFER_SIZE);
	}while(1==1);
	close(fd_out);
	return 0;
}

#endif

void main(){

	int self_socket = make_socket();
	if(self_socket<0){
		printf("\nCould not create socket. Retry!\n");
		return;
	}

	char *server_ip = (char*)malloc(sizeof(char)*IP_STRING_LEN);
	printf("\nEnter File-Server IP Address: ");
	scanf(" %s", server_ip);
	if (connect_server(self_socket, server_ip) < 0){
		printf("\nCould not connect to File-Server.\nMake sure the server is running!\n");
		destroy_socket(self_socket);
		return;
	}
	else{
		printf("\nConnected to File-Server");
	}
   printf("%s", getcwd(NULL, 0)); 
 
	char *msg_buffer = (char*)malloc(sizeof(char)*MSG_BUFFER_SIZE);
	int msg_size = 0;
	int response;
	printf("\n\nEnter 'ENDSESSION' to terminate connection\n");
	do {
		bzero(msg_buffer, MSG_BUFFER_SIZE);
		printf("\nEnter Filename: ");
		scanf(" %[^\n]s", msg_buffer);   
		msg_size = write(self_socket, msg_buffer, MSG_BUFFER_SIZE);
		if (check_termination_init(msg_buffer)){
			msg_size = read(self_socket, msg_buffer, MSG_BUFFER_SIZE);
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
		else if(response==-9){
			printf("\nFile transfer failed unexpectedly\n");
		}
		else if(response==-10){
			printf("File '%s' was not found on server\n", msg_buffer);
		}
		else{
			printf("File downloaded to './downloads/%s'\n", msg_buffer);
		}
	}while(1==1);
}