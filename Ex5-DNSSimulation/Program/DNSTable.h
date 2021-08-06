#ifndef DNS_Table_h
#define DNS_Table_h 

#define DOMAIN_NAME_SIZE 100
#define IP_ADDRESS_SIZE 20

#include<stdlib.h>

// NULL-based Linked-List
struct dns_table{
	char *domain_name;
	char **ips;
	int num_ips;
	struct dns_table *next;
};
typedef struct dns_table DNS_Table;


DNS_Table* make_dns_entry(char *domain_name, char *ip){
	DNS_Table *dns_table = (DNS_Table*)malloc(sizeof(DNS_Table));
	dns_table->domain_name = (char*)malloc(sizeof(char)*DOMAIN_NAME_SIZE);
	dns_table->ips = (char**)malloc(sizeof(char*));
	*(dns_table->ips+0) = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
	memcpy(dns_table->domain_name, domain_name, DOMAIN_NAME_SIZE);
	memcpy(*(dns_table->ips+0), ip, IP_ADDRESS_SIZE);
	dns_table->num_ips = 1;
	dns_table->next = NULL;
	return dns_table;
}


DNS_Table* get_dns_entry(char *domain_name, DNS_Table *table){
	DNS_Table *handle = table;
	while(handle!=NULL){
		if(strcmp(domain_name, handle->domain_name)==0){
			return handle;
		}
		handle = handle->next;
	}
	return NULL;
}


DNS_Table* add_dns_ip(char *domain_name, char *ip, DNS_Table *table){
	DNS_Table *handle = table;
	DNS_Table *entry = NULL;
	DNS_Table *prev = NULL;
	char *ip_parser;
	while(handle!=NULL){
		if(strcmp(handle->domain_name, domain_name)==0){
			entry = handle;
		}
		for(int i=0;i<handle->num_ips;i++){
			if(strcmp(*(handle->ips+i), ip)==0){
				return NULL;     // IP already exists
			}
		}
		prev = handle;
		handle = handle->next;
	}
	if(entry!=NULL){
		entry->ips = (char**)realloc(entry->ips, sizeof(char*)*(entry->num_ips+1));
		*(entry->ips+entry->num_ips) = (char*)malloc(sizeof(char)*IP_ADDRESS_SIZE);
		*(entry->ips+entry->num_ips) = ip;
		entry->num_ips++;
	}
	else{
		entry = make_dns_entry(domain_name, ip);
		if(prev==NULL){
			table = entry;
		}
		else{
			prev->next = entry;
		}
	}
	return table;
}


void display_dns_table(DNS_Table *table){
	DNS_Table *handle = table;
	printf("\n------------------------------------------------------- ");
	printf("\n|       Domain Name        |        IP Address        |");
	printf("\n------------------------------------------------------- ");
	while(handle!=NULL){
		printf("\n|    %-22s", handle->domain_name);
		for(int i=0;i<handle->num_ips;i++){
			if(i==0){
				printf("|    %-22s|", *(handle->ips+i));
			}
			else{
				printf("\n|    %-22s|    %-22s|", " ", *(handle->ips+i));
			}
		}
		printf("\n------------------------------------------------------- ");
		handle = handle->next;
	}
	printf("\n\n");
}

#endif