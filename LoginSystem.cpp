#include "LoginSystem.h"

#include <string.h>

extern Player* PlayerHead;

void LoginSystem(char* message, SOCKET hClntSock){
	char order[30];
	memset(order, 0, sizeof(order));
	printf("LoginSystem() called");
	for(int i= 0; i<30; i++){
		if(message[i]== ' ')
			break;
		order[i] =message[i];
	}
	if(strcmp(order, "NEWMEMBER")== 0){
		NewMember(message, &hClntSock);
	}
	if(strcmp(order, "LOGIN")== 0){
		Login(message, &hClntSock);
	}
}

void NewMember(char* message, SOCKET* hClntSock){
	char id[30];
	char password[30];
	int id_index=0 ;
	int password_index= 0;
	int index= 0;
	FILE* UserList;

	memset(id, 0, sizeof(id));
	memset(password, 0, sizeof(password));

	for(int i=0; i<100; i++){
		if(message[i]== 0)
			break;
		if(message[i]== ' '){
			index++;
			i++;
		}
		if(index==1){
			id[id_index++]= message[i];
		}
		if(index== 2){
			password[password_index++]= message[i];
		}
	}
	UserList= fopen("Members.dat", "at");

	fprintf(UserList, "ID: %s\n", id);
	fprintf(UserList, "PASSWORD: %s\n", password);

	if(fclose(UserList)!= 0){
		printf("file close error\n");
	}
	char servmessage[100]="WELCOME";
	send(*hClntSock, servmessage, 100, 0);
	printf("%s Loged in", id);
}

void Login(char* message, SOCKET* hClntSock){
	char id[30];
	char password[30];
	char UserId[30];
	char UserPassword[30];
	char TempUserId[40];
	char TempUserPassword[40];
	int id_index=0 ;
	int password_index= 0;
	int index= 0;
	char ServMessage[100];
	FILE* UserList= fopen("Members.dat", "rt");
	printf("Login() called\n");
	memset(id, 0, sizeof(id));
	memset(password, 0, sizeof(password));
	memset(TempUserPassword, 0, sizeof(TempUserPassword));
	memset(TempUserId, 0, sizeof(TempUserId));
	memset(UserId, 0, sizeof(UserId));
	memset(UserPassword, 0, sizeof(UserPassword));
	memset(ServMessage, 0, sizeof(ServMessage));

	for(int i=0; i<100; i++){
		if(message[i]== 0)
			break;
		if(message[i]== ' '){
			index++;
			i++;
		}
		if(index==1){
			id[id_index++]= message[i];
		}
		if(index== 2){
			password[password_index++]= message[i];
		}
	}

	Player* TempPlayer= PlayerHead;

	while(TempPlayer!= NULL){
		char usingid[30];
		memset(usingid, 0, sizeof(usingid));
		memcpy(usingid, TempPlayer->GetClntId(), sizeof(usingid));
		if(strcmp(usingid, id)== 0&& TempPlayer->GetConnect()== true){
			printf("error Login again\n");
			strcpy(ServMessage, "ANOTHERUSERISUSING");
			send((*hClntSock), ServMessage, sizeof(ServMessage), 0);
			return;
		}
		TempPlayer= TempPlayer->GetNextNode();
	}
	while(1){
		char* eof;
		char wtf[2]= {10, 0};
		fgets(UserId, 30, UserList);
		eof =fgets(UserPassword, 30, UserList);

		strcpy(TempUserId, "ID: ");
		strcpy(TempUserPassword, "PASSWORD: ");

		strcat(TempUserId, id);
		strcat(TempUserPassword, password);
		strcat(TempUserId, wtf);
		strcat(TempUserPassword, wtf);

		if((strcmp(TempUserId, UserId)==0)&& (strcmp(TempUserPassword, UserPassword)== 0)){
			printf("Login Success\n");
			strcpy(ServMessage, "VALIDACCOUNT");
			send((*hClntSock), ServMessage, 100, 0);
			memset(ServMessage, 0, sizeof(ServMessage));
			int result= Reconnect(id, *hClntSock);
			if(result== 1)
				MakeNewThread(&PlayerHead, (*hClntSock), id);
			break;
		}
		if(eof== NULL){
			printf("Login Fail\n");
			strcpy(ServMessage, "INVALIDACCOUNT");
			send((*hClntSock), ServMessage, 100, 0);
			memset(ServMessage, 0, sizeof(ServMessage));
			break;
		}
		memset(TempUserPassword, 0, sizeof(TempUserPassword));
		memset(TempUserId, 0, sizeof(TempUserId));
	}
}

int Reconnect(char* id, SOCKET Socket){
	Player* PlayerTail= PlayerHead;
	while(PlayerTail!= NULL){
		if(strcmp(PlayerTail->GetClntId(), id)== 0){
			PlayerTail->SetConnection(true);
			PlayerTail->SetSocket(Socket);
			return 0;
		}
		PlayerTail= PlayerTail->GetNextNode();
	}
	return 1;
}
