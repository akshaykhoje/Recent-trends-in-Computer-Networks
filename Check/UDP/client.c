#include<stdio.h>
#include<stdlib.h>

#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<unistd.h>
#include<fcntl.h>

void main(){
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int serv_addr_size = sizeof(serv_addr);

    int msg_size = -1;
    char* msg_buff = (char*)malloc(sizeof(char)*100);
    do{
        printf("\nEnter Message UDP: ");
        scanf(" %s", msg_buff);
        printf("\n MSG: %s", msg_buff);  
        fflush(stdout);
        msg_size = sendto(sock_fd, msg_buff, 100, MSG_CONFIRM, (struct sockaddr*)&serv_addr, serv_addr_size);
        printf("\nHERE: %d", msg_size);   
        fflush(stdout);
        msg_size = recvfrom(sock_fd, msg_buff, 100, MSG_WAITALL, (struct sockaddr*)&serv_addr, &serv_addr_size);   
        printf("\nRECVD: %s", msg_buff);   
        fflush(stdout);
        printf("\nAGAIN ");   
        fflush(stdout);
    }while(msg_size!=0);

    printf("\nEXITED...");
}