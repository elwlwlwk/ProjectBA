#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "LoginSystem.h"

Player* PlayerHead;
MYSQL mysql;

int InitBrSql();

int main(int argc, char** argv){
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	char init[]= "<?xml version=\"1.0\"?><!DOCTYPE cross-domain-policy SYSTEM \"/xml/dtds/cross-domain-policy.dtd\"><cross-domain-policy><site-control permitted-cross-domain-policies=\"master-only\"/><allow-access-from domain=\"clug.kr\" to-ports=\"9997,9998,9999\" /></cross-domain-policy>";


	serv_sock= socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock== -1){
		printf("socket() error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	serv_addr.sin_port= htons(atoi(argv[1]));

	if( bind(serv_sock, (struct sockaddr*) &serv_addr,
	 sizeof(serv_addr))== -1){
		printf("bind()error");
		exit(1);
	}
	printf("call mysql_init()\n");

	mysql_init(&mysql);

	printf("call mysql_read_connect()\n");

	char sqlpass[30];
	memset(sqlpass, 0, sizeof(sqlpass));
	printf("enter sql pass: ");
	scanf("%s", sqlpass);

	if(!mysql_real_connect(&mysql, "localhost", "root", sqlpass, 
"br", 0, (char*)NULL, 0)){
		printf("%s\n", mysql_error(&mysql));
		exit(1);
	}
	else{
		printf("connected\n");
	}

	while(1){

		if( listen(serv_sock, 5)== -1){
			printf("listen() error");
			exit(1);
		}

		clnt_addr_size= sizeof(clnt_addr);
		clnt_sock= accept(serv_sock, (struct sockaddr*) &clnt_addr,
		(socklen_t*)&clnt_addr_size);
		if(clnt_sock== -1){
			printf("accept() error");
			exit(1);
		}

		send(clnt_sock, init, sizeof(init), 0);
		char message[100];
		char order[30];
		memset(message, 0, sizeof(message));

		read(clnt_sock, message, 100);
		printf("call LoginSystem\n");
		LoginSystem(message, clnt_sock);
	}
	close(clnt_sock);
	return 0;
}
