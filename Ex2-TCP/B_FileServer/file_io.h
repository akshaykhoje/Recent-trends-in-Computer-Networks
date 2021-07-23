#ifndef file_io
#define file_io

#include<fcntl.h>
#include<sys/stat.h>

#include "tcp_socket.h"

#define FILE_CHUNK_ACK "FILECHUNKACK"
#define EOF_STRING "ENDFILEHERE"

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
	int fd_out = open("recv", O_CREAT|O_WRONLY, get_default_fileperm());
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