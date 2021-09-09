#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<fcntl.h>
#include<sys/stat.h>


int get_default_fileperm(){
    mode_t curr_umask = umask(022);
    umask(curr_umask);
    return 0666-curr_umask;
}


int main(void) {
    //Stream sockets and rcv()
    
    struct addrinfo hints, *res;
    int sockfd;
    
    char buf[10000];
    int byte_count;

	// www.ssn.edu.in/wp-content/uploads/2021/02/NIRF-2021-SSNCE-ENGG.pdf
	// www.w3.org/WAI/ER/tests/xhtml/testfiles/restores/pdf/dummy.pdf
	// www.africau.edu/images/default/sample.pdf
    
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("www.africau.edu","80", &hints, &res);
    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    printf("Connecting socket...\n");
    connect(sockfd,res->ai_addr,res->ai_addrlen);
    char *header = "GET /images/default/sample.pdf HTTP/1.1\r\nHost: www.africau.edu\r\nConnection: close\r\n\r\n";
	printf("%s", header);
	send(sockfd,header,strlen(header),0);
    printf("Request sent\n");


	int store_fd = open("test.pdf", O_WRONLY | O_CREAT);
	if(store_fd == -1){
		return -4;          // File not Readable
	}       

	do{
		byte_count = recv(sockfd, buf, sizeof(buf), 0);
		printf("Received %d bytes of data\n", byte_count);		
		printf("%s \n ENDS HERE", buf);
		int bytes_written_curr = write(store_fd, buf, strlen(buf));
	}while(byte_count!=0);

	//int fd = 
 
	
	if(!close(store_fd)){
		printf("\nDone");
	}
	else{
		printf("\nError when closing");     // Unexpected Error Occurred
	}

	/*
	GET /admissions/ HTTP/1.1
	Host: www.ssn.edu.in
	User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0
	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,;q=0.8
	Accept-Language: en-US,en;q=0.5
	Accept-Encoding: gzip, deflate
	Connection: keep-alive
	Cookie: _ga=GA1.3.1581703298.1625441352; _gcl_au=1.1.212570893.1626808793; PHPSESSID=hr9d0r7i94kkihsgk7drje2eti; npf_r=; npf_l=www.ssn.edu.in; npf_u=https://www.ssn.edu.in/; npf_fx=1; _gid=GA1.3.1647579336.1631153781; _gat_gtag_UA_199098465_1=1; _gat_gtag_UA_168580093_1=1
	Upgrade-Insecure-Requests: 1
	*/
	
    return 0;
}