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
    int true_flag = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_flag, sizeof(int));

    struct sockaddr_in bind_addr;
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(8080);
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock_fd, (struct sockaddr*)&bind_addr, sizeof(bind_addr));

    struct sockaddr_in client_addr;
    int client_addr_size;

    char *client_ip = (char*)malloc(sizeof(char)*100);
    int client_port;

    int msg_size = -1;
    char* msg_buff = (char*)malloc(sizeof(char)*100);
    do{
        msg_size = recvfrom(sock_fd, msg_buff, 100, MSG_WAITALL, (struct sockaddr*)&client_addr, &client_addr_size);   
        client_port = ntohs(client_addr.sin_port);
        inet_ntop(AF_INET, (void*)&client_addr.sin_addr, client_ip, 100);
        printf("\nFrom %s:%d", client_ip, client_port);  
        printf("\n%s", msg_buff); 
        fflush(stdout);    
        msg_size = sendto(sock_fd, "CHECK", 6, MSG_CONFIRM, (struct sockaddr*)&client_addr, client_addr_size);     
        printf("DONE");
        fflush(stdout);
    }while(msg_size!=0);
}

/*

sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	printf("Hello message sent.\n");

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	char *hello = "Hello from server";
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	int len, n;

	len = sizeof(cliaddr); //len is value/resuslt

	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);
	buffer[n] = '\0';
	printf("Client : %s\n", buffer);
	sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	printf("Hello message sent.\n");
	
	return 0;
}

*/