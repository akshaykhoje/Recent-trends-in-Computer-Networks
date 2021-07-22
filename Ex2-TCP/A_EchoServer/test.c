#include<stdio.h>
#include<stdlib.h>

void main(){
	char *msg = (char*)malloc(sizeof(char)*10);
	while(1==1){
		printf("\nEnter:");
		scanf(" %[^;]s", msg);
		getchar();
		printf("\n%s", msg);
	}
}