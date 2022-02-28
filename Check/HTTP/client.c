#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>

typedef struct addrinfo AddrInfo;

int get_perm(){
    mode_t curr_mask = umask(022);
    umask(curr_mask);
    return 0666-curr_mask;
}

void main(){
    AddrInfo *filter = (AddrInfo*)malloc(sizeof(AddrInfo));
    filter->ai_family = AF_UNSPEC;
    filter->ai_socktype = SOCK_STREAM;

    AddrInfo *serv_addr = (AddrInfo*)malloc(sizeof(AddrInfo));
    getaddrinfo("www.africau.edu", "80", filter, &serv_addr);

    int sock_fd = socket(serv_addr->ai_family, serv_addr->ai_socktype, serv_addr->ai_protocol);
    int retval = connect(sock_fd, serv_addr->ai_addr, serv_addr->ai_addrlen);
    if(retval==0){
        printf("\nConnected\n");
    }

    char *request_head = (char*)malloc(sizeof(char)*1000);
    sprintf(request_head, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", "/images/default/sample.pdf", "www.africau.edu");

    int size = send(sock_fd, request_head, strlen(request_head), 0);
    printf("Sent %d\n", size);
    printf("\n%s\n", request_head);

    char *response = (char*)malloc(sizeof(char)*1000);
    int read_size = -1;
    
    int op_file = open("test.pdf", O_CREAT|O_WRONLY, get_perm());
    do{
        read_size = recv(sock_fd, response, 100, 0);
        printf("\n%s", response);
        fflush(stdout);
        write(op_file, response, read_size);
    }while(read_size!=0);
    close(op_file);
    
    
}