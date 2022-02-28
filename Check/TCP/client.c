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

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int response = connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (response==0){
        printf("\nConnected");
    }

    int msg_size = -1;
    char* msg_buff = (char*)malloc(sizeof(char)*100);
    do{
        printf("\nEnter Message: ");
        scanf(" %s", msg_buff);
        msg_size = send(sock_fd, msg_buff, 100, 0);  
        msg_size = recv(sock_fd, msg_buff, 100, 0);      
    }while(msg_size!=0);
}