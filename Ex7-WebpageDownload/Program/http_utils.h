#ifndef http_utils_h
#define http_utils_h

#include<stdlib.h>
#include<string.h>

#include "tcp_socket.h"

#define HTTP_HEADERLINE_SIZE 50


// Read a file line-by-line
int read_line(int fd, char** buffer){
    char reader[1];
    int line_size = 0;
    char *line_buf = (char*)malloc(sizeof(char)*DEFAULT_BUFFER_SIZE);
    char *temp = NULL;

    int bytes_read = read(fd, reader, 1);
    while(bytes_read!=0 && *(reader)!=10){
        // Until EOF or newline
        *(line_buf+line_size) = *(reader);
        bytes_read = read(fd, reader, 1);
        line_size++;
        temp = (char*)expand_arr_if_full(line_buf, line_size, sizeof(char));
        if(temp==NULL){
            return -100;            // Unexpected Error Occurred
        }
        else{
            line_buf = temp;
        }
    }

    *(buffer) = line_buf;
    return line_size;
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

char* check_response_status(char *response_file){
	int read_fd = open(response_file, O_RDONLY);
	char *line = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	int line_size;
	line_size = read_line(read_fd, line);
	while(line_size!=0){
		
		line_size = read_line(read_fd, line);
	}
	char *status_code = (char*)malloc(sizeof(char)*HTTP_HEADERLINE_SIZE);
	float version;
	char *temp_1 = (char*)malloc(sizeof(char)*HTTP_HEADERLINE_SIZE);
	char *temp_2 = (char*)malloc(sizeof(char)*HTTP_HEADERLINE_SIZE);
	printf("%s\n", response);
	sscanf(response, "%HTTP/%f %[^ ]%s", version, status_code, temp_1);
	printf("->V%fS%s", version, status_code);
	return response;
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

