#ifndef http_utils_h
#define http_utils_h

#include<stdlib.h>
#include<string.h>

#include "tcp_socket.h"

#define HTTP_HEADERLINE_SIZE 500
#define HTTP_PORT_STRING "80"
#define CONTENT_TYPE_ATTR_NAME "Content-Type"
#define CONTENT_PDF_VALUE "application/pdf"
#define PDF_START_SYMBOL "%PDF"


// Read a file line-by-line
int read_line(int fd, char** buffer){
    char reader[2];
    int line_size = 0;
    char *line_buf = (char*)malloc(sizeof(char)*HTTP_HEADERLINE_SIZE);

    int bytes_read = read(fd, reader, 1);
    while(bytes_read!=0 && *(reader)!=10){
        // Until EOF or newline
        *(line_buf+line_size) = *(reader);
        bytes_read = read(fd, reader, 1);
        line_size++;
    }
    *(buffer) = line_buf;
    return line_size;
}

short startswith(char *prefix, char *string){
    return strncmp(prefix, string, strlen(prefix)) == 0;
}

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

short check_response_status(char *response_file, char **response_head){
	// Currently ensures successful PDF response
	// File readers
	int read_fd = open(response_file, O_RDONLY);
	char *line = (char*)malloc(sizeof(char)*HTTP_HEADERLINE_SIZE);
	// Parameter readers
	int status_code = -1;
	float version = -1;
	short type_read = 0;
	short body_start = 0;
	int response_head_idx = 0;
	char *type = (char*)malloc(sizeof(char)*HTTP_HEADERLINE_SIZE);
	//type = NULL;
	int line_size = read_line(read_fd, &line);
	while(line_size!=0){
		fflush(stdout);
		if(version==-1){
			sscanf(line, "HTTP/%f %d", &version, &status_code);
		}
		else if(!type_read && startswith(CONTENT_TYPE_ATTR_NAME, line)){
			sscanf(line, "Content-Type: %s", type);
			type_read = 1;
		}
		else if(!body_start){
			// Store the response head
			if(startswith(PDF_START_SYMBOL, line)){
				body_start = 1;
			}
			else{
				memcpy(*response_head, line, line_size);
			}
		}
		line_size = read_line(read_fd, &line);
	}
	close(read_fd);
	// Return validation result
	if(status_code==200 && strcmp(CONTENT_PDF_VALUE, type)==0){
		// Verified. PDF
		return 1;
	}
	return 0;
}

/*
HTTP/1.1 200 OK
Date: Sat, 11 Sep 2021 02:15:54 GMT
Content-Type: application/pdf
Content-Length: 3028
Connection: close
Last-Modified: Fri, 24 Feb 2017 17:42:38 GMT
ETag: "58b0708e-bd4"
Expires: Sun, 24 Apr 2022 02:07:03 GMT
Cache-Control: max-age=31536000
Host-Header: 8441280b0c35cbc1147f8ba998a563a7
X-Proxy-Cache-Info: DT:1
CF-Cache-Status: HIT
Age: 12096531
Accept-Ranges: bytes
Report-To: {"endpoints":[{"url":"https:\/\/a.nel.cloudflare.com\/report\/v3?s=QPStsF265LuROHuiaXlOcuf1V4hCQzjbSswac1pvFrW%2Bjks9%2BV7cfeocSAxy%2BUz2mLgeFzQCcHgmt%2F49PIbg20yhJ2%2BvHhFfM%2FIHGZya3wUa7PiLAfboQDqWcL048FXwaOY%3D"}],"group":"cf-nel","max_age":604800}
NEL: {"success_fraction":0,"report_to":"cf-nel","max_age":604800}
Server: cloudflare
CF-RAY: 68cd64131e434b1c-HYD
alt-svc: h3=":443"; ma=86400, h3-29=":443"; ma=86400, h3-28=":443"; ma=86400, h3-27=":443"; ma=86400
*/

#endif

