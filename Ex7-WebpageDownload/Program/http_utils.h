#ifndef http_utils_h
#define http_utils_h

#include<stdlib.h>
#include<string.h>

#include "tcp_socket.h"

char* get_hostname(char *web_address){
	char *hostname = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	char *remain = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	sscanf(web_address, "%[^/]/%s", hostname, remain);
	return hostname;
}

char* get_resource_path(char *web_address){
	char *remain = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	char *path = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	sscanf(web_address, "%[^/]%s", remain, path);
	return path;
}

char* prepare_get_header(char* hostname, char* resource_path){
	char *header = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	sprintf(header, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", resource_path, hostname);
	return header;
}

#endif

