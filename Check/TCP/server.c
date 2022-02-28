#include<stdio.h>
#include<stdlib.h>

#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<fcntl.h>

void main(){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int true_flag = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_flag, sizeof(int));

    struct sockaddr_in bind_addr;
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(8080);
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock_fd, (struct sockaddr*)&bind_addr, sizeof(bind_addr));

    listen(sock_fd, 5);

    struct sockaddr_in client_addr;
    int client_addr_size;
    int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_addr_size);

    char *client_ip = (char*)malloc(sizeof(char)*100);
    inet_ntop(AF_INET, (void*)&client_addr.sin_addr, client_ip, 100);
    int client_port = (int)ntohs(client_addr.sin_port);
    printf("\n%s\n%d", client_ip, client_port);

    int msg_size = -1;
    char* msg_buff = (char*)malloc(sizeof(char)*100);
    do{
        msg_size = recv(client_fd, msg_buff, 100, 0);   
        printf("\n%s", msg_buff);     
        msg_size = send(client_fd, msg_buff, 100, 0);     
    }while(msg_size!=0);
}