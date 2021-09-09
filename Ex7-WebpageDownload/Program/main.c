#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>

#ifndef tcp_socket_h
	#include "tcp_socket.h"
#endif

#ifndef http_utils_h
	#include "http_utils.h"
#endif


int get_default_fileperm(){
    mode_t curr_umask = umask(022);
    umask(curr_umask);
    return 0666-curr_umask;
}


int main(int argc, char **argv) {
    if(argc<2){
    	printf("\nSupply web address\n");
    	exit(1);
    }
    // Partition web
    char *web_address = *(argv+1);
    char *hostname = get_hostname(web_address);
    char *resource_path = get_resource_path(web_address);
    
    // Collect server socket details
    AddrInfo *socket_details;        
    // socket-filter
    AddrInfo socket_filter;
    memset(&socket_filter, 0, sizeof(socket_details));
    socket_filter.ai_family = AF_UNSPEC;
    socket_filter.ai_socktype = SOCK_STREAM;
    getaddrinfo("www.africau.edu","80", &socket_filter, &socket_details);
    
    char *data_buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    int data_size;
    int response;

	// www.ssn.edu.in/wp-content/uploads/2021/02/NIRF-2021-SSNCE-ENGG.pdf
	// www.w3.org/WAI/ER/tests/xhtml/testfiles/restores/pdf/dummy.pdf
	// www.africau.edu/images/default/sample.pdf
    
    int client_socket = make_socket(socket_details);
    printf("\nConnecting to server...\n");
    response = connect_server(client_socket, socket_details);
    if(response<0){
    	printf("\nCould not connect to server. Retry!");
    }
	
    // Prepare HTTP header
    char *header = prepare_get_header(hostname, resource_path);	
	// Send Request
	send(client_socket, header, strlen(header), 0);
    printf("\nHTTP Request Sent\n");
    printf("\nRequest Header\n%s", header);
	
	// Get response
	char *store_as = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	printf("\nName your download: ");
	scanf(" %s", store_as);
	sprintf(store_as, "%s.pdf", store_as);
	int store_fd = open(store_as, O_WRONLY | O_CREAT, get_default_fileperm());
	if(store_fd == -1){
		return -4;          // File not Readable
	}     
	do{
		data_size = recv(client_socket, data_buffer, sizeof(data_size), 0);	
		data_size = write(store_fd, data_buffer, data_size);
	}while(data_size!=0);
	
	// Close file and sockets
	if(!close(store_fd)){
		printf("\nPDF downloaded as '%s'\n", store_as);
	}
	else{
		printf("\nError when closing\n");     // Unexpected Error Occurred
	}	
	destroy_socket(client_socket);
    return 0;
}
