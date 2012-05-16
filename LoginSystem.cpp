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
	Player Dummy;
	FILE* UserList;

	char query[100];

	memset(query, 0, sizeof(query));
	printf("%s", message);
	printf("NewMemberCalled\n");
	memset(id, 0, sizeof(id));
	memset(password, 0, sizeof(password));
	printf("memset over");

	int c[10];
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

	sprintf(query, "select * from br_account where ID= '%s'", id);

	MYSQL_RES *res;
	int fields;

	Dummy.SendQuery(query, &res, &fields);
	if(res!= NULL){
		send(*hClntSock, "overlaped id", 13, 0);
		mysql_free_result(res);
		return;
	}

	memset(query, 0, sizeof(query));

	sprintf(query, 
"insert into br_account values('%s', '%s', 'NULL', 'NULL');", 
id, password);

	printf("Send Query: %s\n", query);
	Dummy.SendQuery(query, &res, &fields);
	mysql_free_result(res);

	memset(query, 0, sizeof(query));

	sprintf(query, 
"insery into br_characters values(%d, %d, '%s', %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d, %d", 
0, 0, id, 1000, 1000, 1000, 1000, "cabin", -170, 18, 0, 0, 0, 0, 0);

	printf("Send Query: %s\n", query);
	Dummy.SendQuery(query, &res, &fields);
	mysql_free_result(res);

/*
	UserList= fopen("Members.dat", "at");

	fprintf(UserList, "ID: %s\n", id);
	fprintf(UserList, "PASSWORD: %s\n", password);

	if(fclose(UserList)!= 0){
		printf("file close error\n");
	}
*/
	char servmessage[100]="WELCOME";
	send(*hClntSock, servmessage, 100, 0);
	printf("NewMember: %s", id);
}

void Login(char* message, SOCKET* hClntSock){

	char id[30];
	char password[30];
	char tempMessage[100];

	char query[100];
	MYSQL_RES *res= NULL;
	int fields= 0;
	Player Dummy;

	memset(id, 0, sizeof(id));
	memset(password, 0, sizeof(password));
	memset(tempMessage, 0, sizeof(tempMessage));
	memset(query, 0, sizeof(query));

	for(int i= 0, k= 0, m= 0; i<100&& message[i]!= 0; i++){
		if(message[i]== ' '){
			i++;
			k++;
			m= 0;
		}
		if(k== 1){
			id[m++]= message[i];
		}
		else if(k== 2){
			password[m++]= message[i];
		}
	}

	printf("Login from %s %s\n", id, password);

	sprintf(query, "select * from BR_Account where ID='%s'and Pass='%s'\n", id, password);

	printf("query is %s\n", query);
	Dummy.SendQuery(query, &res, &fields);

	printf("%d, %d\n", res, fields);

	if(res== NULL|| fields== 0){
		printf("res is null");
		sprintf(tempMessage, "INVALIDACCOUNT");
		send((*hClntSock), tempMessage, 100, 0);
		return;
	}
	else{
		MYSQL_ROW row;
		row= mysql_fetch_row(res);

		Player* PlayerTail= PlayerHead;

		while(PlayerTail!= NULL){
			if(strcmp(PlayerTail->GetClntId(), id)== 0){
				printf("in alreadyusr\n");
				break;
			}
			PlayerTail= PlayerTail->GetNextNode();
		}
		if(PlayerTail== NULL){
			printf("PlayerTail is NULL\n");
		}

		if(PlayerTail!= NULL&& PlayerTail->GetConnect()== true){
                        printf("another user is using\n");
                        sprintf(tempMessage, "ANOTHERUSERISUSING");
                        send((*hClntSock), tempMessage, 100, 0);
			return;
                }


		printf("Login Success\n");
		int result= Reconnect(id, (*hClntSock));
		if(result== 1){
			printf("call MakeNewThread\n");
			sprintf(tempMessage, "VALIDACCOUNT");
			printf("Send %s to %d\n", tempMessage, *hClntSock);
			send((*hClntSock), tempMessage, 100, 0);
			MakeNewThread(&PlayerHead, (*hClntSock), id);
		}
	}

/*
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

		printf("*************%d", (*hClntSock));
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
*/
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
