#include "Player.h"
#include <stdio.h>
#include <stdlib.h>

void Player::Disconnect(){
	connected= false;
	close(hClnt);

	char query[100];
	memset(query, 0, sizeof(query));
	sprintf(query, 
"update br_characters set arena= '%s', PosX= %d, PosY= %d where CharName= \'%s\';", 
arena ,Posx, Posy, name);
	printf("Send Query %s\n", query);
	MYSQL_RES* res;
	int fields;
	SendQuery(query, &res, &fields);

	printf("call mysql_free_result()\n");
	mysql_free_result(res);
}

Player::Player(SOCKET hClnt, char* name){
	this->hClnt= hClnt;
	memset(this->name, 0, sizeof(this->name));
	memcpy(this->name, name, 30);
	NextNode= NULL;
	connected= true;
	Posx= Posy= 0;
	memset(arena, 0, sizeof(arena));

	char query[100];
	memset(query, 0, sizeof(0));
	sprintf(query, "select * from br_characters where CharName= '%s'", 
name);

	MYSQL_RES* res;
	int fields;
	MYSQL_ROW row;

	printf("init Call SendQuery()\n");
	SendQuery(query, &res, &fields);

	printf("init call fetch_row()\n");
	row= mysql_fetch_row(res);

	printf("init arena is %s\n", row[7]);
	strcpy(arena, row[7]);

	printf("init call mysql_free_result()\n");
	mysql_free_result(res);
}


Player::~Player(void)
{
}

Player* Player::GetNextNode(){
	return NextNode;
}

char* Player::GetClntId(){
	return name;
}

void Player::SetNextNode(Player* NextNode){
	this->NextNode= NextNode;
}

int Player::GetClntMessage(char* buffer){

	printf("GetClntMessage() called()\n");
	printf("%d\n", connected);
	while(connected== false){
		sleep(1); //connect는 false에서 true로 loginsystem에서 바꿔준다.
	}
	int index= 0;
	int result;
	memset(buffer, 0, sizeof(buffer));
	printf("before get message\n");
	do{
		result= read(hClnt, &buffer[index], sizeof(char));
		//printf("getmessage %c\n", buffer[index]);

	}while(buffer[index++]!= 0);
	printf("%s's result is %d\n",name, result);
	if(result== -1|| result== 0){
		Disconnect();
	}

	return result;
}

void Player::SendClntMessage(char* message){
	int len=0;
	len= strlen(message);
	if(connected== true){
		printf("Send Message to %s: %s\n", name, message);
		send(hClnt, message, len+1, 0);
	}
}

void Player::SetSocket(SOCKET Socket){
	hClnt= Socket;
}

bool Player::GetConnect(){
	return connected;
}

void Player::SetConnection(bool connection){
	connected= connection;
}


int Player::PlayerMessageProc(char* message){

	char recvmes[10][30];

	for(int i=0; i<10; i++){
	memset(recvmes[i], 0, 30);
	}

	printf("PlayerMessageProc() called\n");
	for(int i= 0, k= 0, m= 0; message[i]!= 0&& i<100; i++){
		if(message[i]== ' '){
			m= 0;
			k++;
			continue;
		}
		recvmes[k][m++]= message[i];
	}
	printf("message init success\n");
	if(strcmp(recvmes[1], "MOVE")== 0){
		Posx= atoi(recvmes[3]);
		Posy= atoi(recvmes[4]);
	}

	else if(strcmp(recvmes[1], "CHMAP")== 0){
		memset(arena, 0, sizeof(arena));
		strcpy(arena, recvmes[3]);
	}

	return 0;
}

void Player::SendPosInfo(Player* targetPlayer){
	char tempMessage[100];
	memset(tempMessage, 0, sizeof(tempMessage));

	sprintf(tempMessage, "MOVE %s %d %d", name, Posx, Posy);
	if(strcmp(targetPlayer->GetClntId(), name)!= 0)
		targetPlayer->SendClntMessage(tempMessage);

}

void Player::SetPos(int x, int y){
	Posx= x;
	Posy= y;
}

int Player::SendQuery(char* query, MYSQL_RES** res, int* fields){
	extern MYSQL mysql;
	mysql_real_query(&mysql, query, strlen(query));
	*res= mysql_store_result(&mysql);
	*fields= mysql_field_count(&mysql);
	return 0;
}
