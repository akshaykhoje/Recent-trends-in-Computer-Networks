#include<fcntl.h>

#include "tcp_socket.h"

#define FILE_CHUNK_ACK "FILECHUNKACK"
#define EOF_STRING "ENDFILEHERE"

short check_chunk_ack(char *msg){
	return (strcmp(msg, FILE_CHUNK_ACK)==0);
}

short check_eof(char *msg){
	return (strcmp(msg, EOF_STRING)==0);
}

void send_file(char *filepath, int sock_fd){
	int fd_in = open(filepath, O_RDONLY);
	if (fd_in<0){
		return -6;   // Could not open read-file
	}
	char *data = (char*)malloc(sizeof(char)*FILE_CHUNK_SIZE);
	int read_size = read(fd, buffer, MSG_BUFFER_SIZE);
	int write_size = 0;
	do{
		write_size = write(sock_fd, data, read_size);
		// Wait for acknowledgement
		read_size = read(sock_fd, data, read_size);
		if (!check_chunk_ack(data)){
			return -7;    // Corrupt occurred while sending file
		}
		read_size = read(fd_in, data, MSG_BUFFER_SIZE);		
	}while(read_size!=0);
	write_size = write(sock_fd, EOF_STRING, sizeof(EOF_STRING));
	close(fd_in);
}


void receive_file(char *filepath, int sock_fd){
	int fd_out = open(filepath, O_WRONLY);
	if(fd_out<0){
		return -8;   // Could not open write-file
	}
	char *data = (char*)malloc(sizeof(char)*);
	int read_size = read(sock_fd, buffer, MSG_BUFFER_SIZE);
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
}